#include "teste.hpp"
#include <chrono>
#include <iomanip>  // Para usar std::setprecision
#include <vector>
#include <map>  
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <tuple>
#include <utility> // Para std::pair
#include <numeric> // Para std::iota
#include <iterator> // Para std::inserter
#include <thread>
#include <mutex>

using namespace std;

const double THRESHOLD_SIMILARIDADE = 0.5;
const int maxComb = 3;

mutex mutexArquivo;  // Mutex para proteger o acesso ao arquivo de saída
mutex mutexContadores;  // Mutex para proteger os contadores de acertos e erros
mutex mutexRelevancia;

void selecionarLinhasAleatorias(const string& inputFile, const string& outputFile, int numLinhas) {
    ifstream arquivoEntrada(inputFile);
    ofstream arquivoSaida(outputFile);
    vector<string> linhas;
    string linha;
    if (!arquivoEntrada.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada!" << endl;
        return;
    }
    while (getline(arquivoEntrada, linha)) {
        linhas.push_back(linha);
    }
    arquivoEntrada.close();

    // Inicializa a semente para o gerador de números aleatórios
    srand(time(nullptr));
    // Seleciona aleatoriamente as linhas e escreve no arquivo de saída
    for (int i = 0; i < numLinhas; ++i) {
        int indiceAleatorio = rand() % linhas.size();
        arquivoSaida << linhas[indiceAleatorio] << endl;
    }
    arquivoSaida.close();
}

// Função para calcular a similaridade entre duas linhas da base de dados, ignorando as colunas 0, 2, 4, 6, 8 e a última coluna.
double calcularSimilaridade(const vector<int>& linha1, const vector<int>& linha2) {
    double similaridade = 0.0;
    int count = 0;
    // Itera sobre as colunas relevantes (1, 3, 5, 7)
    for (long unsigned int i = 1; i < linha1.size() - 1; i += 2) {
        // Considera apenas as colunas que representam valores (não os naipes)
        if (linha1[i] == linha2[i]) {
            similaridade += 1.0;
        }
        count++;
    }
    // Normaliza a similaridade pelo número de colunas consideradas
    if (count > 0) {
        similaridade /= count;
    }
    return similaridade;
}


// Função para gerar combinações
vector<vector<int>> combinacoes(const vector<int>& indices, int n) {
    vector<vector<int>> result;
    vector<bool> v(indices.size());
    fill(v.begin(), v.begin() + n, true);
    do {
        vector<int> comb;
        for (long unsigned int i = 0; i < indices.size(); ++i) {
            if (v[i]) {
                comb.push_back(indices[i]);
            }
        }
        result.push_back(comb);
    } while (prev_permutation(v.begin(), v.end()));
    return result;
}

// Função que será executada em paralelo por diferentes threads.
// Ela calcula a interseção das linhas e atualiza a relevância das classes.
void calcularSuportePorCombinacao(const vector<set<int>>& linhas, 
                                  const vector<int>& comb,
                                  const unordered_map<int, set<int>>& tabelaHashClasses,
                                  unordered_map<int, double>& relevanciaClasse,
                                  int totalLinhas) {
    // Inicializa a interseção com o conjunto de linhas da primeira feature na combinação
    set<int> intersecao = linhas[comb[0]];

    // Itera sobre as outras features na combinação, calculando a interseção sucessiva
    for (size_t idx = 1; idx < comb.size(); ++idx) {
        set<int> tempIntersecao;
        set_intersection(intersecao.begin(), intersecao.end(),
                         linhas[comb[idx]].begin(), linhas[comb[idx]].end(),
                         inserter(tempIntersecao, tempIntersecao.begin()));
        intersecao = tempIntersecao;
    }

    // Itera sobre todas as classes no mapa `tabelaHashClasses`
    for (const auto& classe : tabelaHashClasses) {
        set<int> linhasClasse = classe.second; // Conjunto de linhas onde a classe aparece
        set<int> linhasIntersecao;
        
        // Calcula a interseção entre as linhas da combinação e as linhas da classe
        set_intersection(intersecao.begin(), intersecao.end(),
                         linhasClasse.begin(), linhasClasse.end(),
                         inserter(linhasIntersecao, linhasIntersecao.begin()));
        
        // Se a interseção não for vazia, significa que há suporte para essa classe
        if (!linhasIntersecao.empty()) {
            double suporte = static_cast<double>(linhasIntersecao.size()) / totalLinhas;
            
            // Protege o acesso a `relevanciaClasse` usando um mutex.
            // `std::lock_guard` automaticamente tranca o mutex ao ser criado
            // e o destranca ao sair do escopo.
            std::lock_guard<std::mutex> lock(mutexRelevancia);
            
            // Atualiza a relevância da classe com o suporte calculado
            relevanciaClasse[classe.first] += suporte;
        }
    }
}

// Função principal que avalia a classe combinatória
int avaliarClasseCombinatoria(const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                              const unordered_map<int, set<int>>& tabelaHashClasses,
                              const vector<tuple<int, int>>& featuresLinha, int totalLinhas) {
    unordered_map<int, double> relevanciaClasse; // Mapa para armazenar a relevância de cada classe
    vector<set<int>> linhas; // Vetor para armazenar as linhas correspondentes a cada feature

    // Para cada feature na linha, busca as linhas correspondentes na `tabelaHash`
    for (const auto& tupla : featuresLinha) {
        int coluna = get<0>(tupla);
        int valor = get<1>(tupla);
        set<int> linhasFeature = buscarFeature(tabelaHash, coluna, valor);
        
        // Se o conjunto de linhas não estiver vazio, adiciona ao vetor `linhas`
        if (!linhasFeature.empty()) {
            linhas.push_back(linhasFeature);
        }
    }

    int numLinhas = linhas.size(); // Número de features presentes na linha
    vector<int> indices(numLinhas);
    iota(indices.begin(), indices.end(), 0); // Preenche `indices` com valores de 0 a numLinhas-1

    vector<thread> threads; // Vetor para armazenar as threads criadas

    // Loop para considerar combinações de 1 até 3 features
    for (int interacao = 1; interacao <= maxComb; interacao++) {
        // Gera todas as combinações de `interacao` elementos dos índices
        auto combs = combinacoes(indices, interacao);
        
        // Para cada combinação, cria uma nova thread que executa `calcularSuportePorCombinacao`
        for (const auto& comb : combs) {
            threads.push_back(thread(calcularSuportePorCombinacao, ref(linhas), comb, 
                                     ref(tabelaHashClasses), ref(relevanciaClasse), totalLinhas));
        }
    }

    // `join()` é chamado em cada thread para garantir que o programa espere
    // todas as threads terminarem antes de prosseguir.
    for (auto& t : threads) {
        t.join();
    }

    // Converte o mapa `relevanciaClasse` em um vetor de pares para ordenação
    vector<pair<int, double>> suporteClasses(relevanciaClasse.begin(), relevanciaClasse.end());

    // Ordena as classes pelo suporte em ordem decrescente
    sort(suporteClasses.begin(), suporteClasses.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });

    // Retorna a classe com maior suporte ou -1 se não houver classes
    if (!suporteClasses.empty()) {
        return suporteClasses.front().first;
    } else {
        return -1;
    }
}

double calcularSuporteBucket(const vector<pair<vector<int>, int>>& bucket,
                             int totalLinhas, int maxComb) {
    unordered_map<int, int> contagemClasses;  // Para contar a frequência das classes no bucket
    vector<tuple<int, int>> featuresLinha;

    // Preenche as features da linha para análise
    for (const auto& par : bucket) {
        const vector<int>& linha = par.first;
        int classe = par.second;

        // Incrementa a contagem da classe
        contagemClasses[classe]++;
        
        // Adiciona features da linha
        for (size_t i = 0; i < linha.size(); ++i) {
            featuresLinha.push_back(make_tuple(i, linha[i]));
        }
    }

    double suporteTotal = 0.0;

    // Calcula o suporte baseado na contagem das classes
    for (const auto& entry : contagemClasses) {
        int frequencia = entry.second;

        // Calcula o suporte da classe normalizado
        double suporteClasse = static_cast<double>(frequencia) / static_cast<double>(totalLinhas);
        suporteTotal += suporteClasse;
    }

    // Normaliza o suporte total com base no máximo número de combinações
    suporteTotal /= maxComb;

    return suporteTotal;
}

<<<<<<< Updated upstream

// Função para criar buckets com base na similaridade
unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> criarBucketsComSimilaridade(const string& nomeArquivoTeste, int& totalLinhas) {
    ifstream arquivoTeste(nomeArquivoTeste);
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets;
    vector<pair<vector<int>, int>> linhas; // Armazena linhas e números das linhas
    string linha;
    int bucketIndex = 0;
    totalLinhas = 0;
    int numeroLinha = 1;

    // Lê o arquivo de teste e armazena as linhas com seus números
    while (getline(arquivoTeste, linha)) {
        stringstream ss(linha);
        string item;
        vector<int> linhaValores;

        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }

        linhas.push_back({linhaValores, numeroLinha});
        totalLinhas++;
        numeroLinha++;
    }

    // Organiza as linhas em buckets com base na similaridade
    for (const auto& linhaAtual : linhas) {
        bool adicionado = false;
=======
// Mutexes para proteger o acesso a variáveis compartilhadas
mutex mutexBuckets, mutexSuportesExistentes;

unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> criarBucketsPorThread(
    const vector<pair<vector<int>, int>>& linhas, 
    const set<int>& classesDistintas,
    int inicio, int fim) {

    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets;
    unordered_map<int, double> suportesExistentes; // Armazena suportes já calculados localmente

    // Inicializa os buckets com as classes distintas
    for (int classe : classesDistintas) {
        buckets[classe] = make_pair(vector<pair<vector<int>, int>>(), 0.0);
    }

    // Processa o subconjunto de linhas
    for (int i = inicio; i < fim; ++i) {
        const auto& linha = linhas[i];
        int melhorBucket = -1;
        double maiorSimilaridade = 0.0;

>>>>>>> Stashed changes
        for (auto& bucket : buckets) {
            for (const auto& linhaBucket : bucket.second.first) {
                if (calcularSimilaridade(linhaAtual.first, linhaBucket.first) >= THRESHOLD_SIMILARIDADE) {
                    bucket.second.first.push_back(linhaAtual);
                    adicionado = true;
                    break;
                }
            }
            if (adicionado) break;
        }

        if (!adicionado) {
            buckets[bucketIndex] = make_pair(vector<pair<vector<int>, int>>{linhaAtual}, 0.0);
            bucketIndex++;
        }
    }

<<<<<<< Updated upstream
    // Calcula o suporte para cada bucket
=======
    return buckets;
}

// Função principal para criar buckets com base na similaridade utilizando threads
unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> criarBucketsComSimilaridade(
    const string& nomeArquivoTeste, int& totalLinhas, int maxComb,
    const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
    const unordered_map<int, set<int>>& tabelaHashClasses) {

    ifstream arquivoTeste(nomeArquivoTeste);
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets;
    unordered_map<int, double> suportesExistentes; // Armazena suportes já calculados
    vector<pair<vector<int>, int>> linhas;
    set<int> classesDistintas;
    totalLinhas = 0;

    if (!arquivoTeste) {
        cerr << "Erro ao abrir o arquivo de teste." << endl;
        return {};
    }

    string linha;
    while (getline(arquivoTeste, linha)) {
        stringstream ss(linha);
        string item;
        vector<int> linhaValores;
        int classe;

        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }

        classe = linhaValores.back();
        classesDistintas.insert(classe);
        linhas.push_back({linhaValores, classe});
        totalLinhas++;
    }

    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 1;  // Fallback para pelo menos uma thread se a consulta falhar
    }

    int linhasPorThread = totalLinhas / numThreads;
    vector<thread> threads;
    vector<unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>> resultadosParciais(numThreads);

    // Cria threads para processar subconjuntos de linhas
    for (int i = 0; i < numThreads; ++i) {
        int inicio = i * linhasPorThread;
        int fim = (i == numThreads - 1) ? totalLinhas : inicio + linhasPorThread;

        threads.push_back(thread([&, i, inicio, fim] {
            resultadosParciais[i] = criarBucketsPorThread(linhas, classesDistintas, inicio, fim);
        }));
    }

    // Aguarda todas as threads terminarem
    for (auto& t : threads) {
        t.join();
    }

    // Combina os resultados parciais dos buckets de todas as threads
    for (const auto& resultadoParcial : resultadosParciais) {
        for (const auto& bucketParcial : resultadoParcial) {
            int classe = bucketParcial.first;
            lock_guard<mutex> lock(mutexBuckets); // Protege o acesso ao mapa de buckets
            buckets[classe].first.insert(
                buckets[classe].first.end(), 
                bucketParcial.second.first.begin(), 
                bucketParcial.second.first.end()
            );
        }
    }

    // Calcula os suportes de todos os buckets
>>>>>>> Stashed changes
    for (auto& bucket : buckets) {
        const vector<pair<vector<int>, int>>& bucketLinhas = bucket.second.first;
        double suporte = calcularSuporteBucket(bucketLinhas, totalLinhas, maxComb);
        bucket.second.second = suporte;
<<<<<<< Updated upstream
=======
        suportesExistentes[bucket.first] = suporte;
    }

    // Salva os buckets em um arquivo de saída
    ofstream arquivoBuckets("buckets_output.txt");
    for (const auto& bucket : buckets) {
        arquivoBuckets << "Bucket " << bucket.first << " (Suporte: " << bucket.second.second << "):\n";
        for (const auto& linhaBucket : bucket.second.first) {
            arquivoBuckets << "Linha: ";
            for (const auto& valor : linhaBucket.first) {
                arquivoBuckets << valor << " ";
            }
            arquivoBuckets << "Classe: " << linhaBucket.second << "\n";
        }
        arquivoBuckets << "\n";
>>>>>>> Stashed changes
    }

    return buckets;
}



// Função para avaliar a classe de uma linha
int avaliarClasse(const vector<int>& linha, 
                  const unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>& buckets,
                  const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                  const unordered_map<int, set<int>>& tabelaHashClasses,
                  int totalLinhas) {
    // Verifica se a linha está em algum bucket
    for (const auto& bucket : buckets) {
    // Itera sobre as linhas no bucket
    for (const auto& linhaBucket : bucket.second.first) {
        // Cria uma cópia da linha do bucket
        vector<int> linhaBucketCopy = linhaBucket.first;
        linhaBucketCopy.pop_back();
        // Se a linha do bucket e a linha a ser comparada forem iguais
        if (linha == linhaBucketCopy) {
            // Linha está em um bucket, retorna a classe associada ao bucket
            return bucket.second.second;
        }
    }
}

    // Se a linha não estiver em um bucket, prossegue com a análise combinatória
    vector<tuple<int, int>> featuresLinha;
    for (size_t i = 0; i < linha.size(); ++i) {
        featuresLinha.push_back(make_tuple(i + 1, linha[i])); // Ajusta para a estrutura de tupla (coluna, valor)
    }

    return avaliarClasseCombinatoria(tabelaHash, tabelaHashClasses, featuresLinha, totalLinhas);
}

// Função auxiliar para processar um subconjunto de linhas
void processarLinhas(int threadId, vector<string>& linhas, int inicio, int fim,
                     const unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>& buckets,
                     const unordered_map<tuple<int, int>, set<int>>& tabelaHashTreino,
                     const unordered_map<int, set<int>>& tabelaHashClassesTreino,
                     int totalLinhas, int& acertos, int& erros, ofstream& arquivoSaida) {
    for (int i = inicio; i < fim; ++i) {
        // Mensagem de depuração
        threadId = threadId;

        stringstream ss(linhas[i]);
        string item;
        vector<int> linhaValores;

        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }

        if (!linhaValores.empty()) {
            int classeOriginal = linhaValores.back();
            linhaValores.pop_back();
            int classeAtribuida = avaliarClasse(linhaValores, buckets, tabelaHashTreino, tabelaHashClassesTreino, totalLinhas);

            {
                lock_guard<mutex> lock(mutexArquivo);
                arquivoSaida << "Linha " << (i + 1) << ": Classe Atribuída = " << classeAtribuida << endl;
            }
            {
                lock_guard<mutex> lock(mutexContadores);
                if (classeAtribuida == classeOriginal) {
                    acertos++;
                } else {
                    erros++;
                }
            }
        }
    }
}

// Função para testar o algoritmo com um arquivo de teste usando threads
void teste(const string& nomeArquivoTeste) {
    ifstream arquivoTeste(nomeArquivoTeste);
    auto inicio = chrono::high_resolution_clock::now();

    int totalLinhas;
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets = criarBucketsComSimilaridade(nomeArquivoTeste, totalLinhas);

    if (!arquivoTeste) {
        cerr << "Erro ao abrir o arquivo de teste." << endl;
        return;
    }

    ofstream arquivoSaida("dataset/output.txt");
    if (!arquivoSaida) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        return;
    }

    // Variáveis para contagem de acertos e erros
    int totalLinhasArquivo = 0;
    int acertos = 0;
    int erros = 0;

    // Lê todas as linhas do arquivo em um vetor
    vector<string> linhas;
    string linha;
    while (getline(arquivoTeste, linha)) {
        linhas.push_back(linha);
        totalLinhasArquivo++;
    }

    // Define o número de threads com base na capacidade do hardware
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 1;  // Fallback para pelo menos uma thread se a consulta falhar
    }

    int linhasPorThread = totalLinhasArquivo / numThreads;
    vector<thread> threads;

    // Cria threads para processar subconjuntos de linhas
    for (int i = 0; i < numThreads; ++i) {
        int inicio = i * linhasPorThread;
        int fim = (i == numThreads - 1) ? totalLinhasArquivo : inicio + linhasPorThread;

        threads.push_back(thread(processarLinhas, i, ref(linhas), inicio, fim, ref(buckets),
                                 ref(tabelaHashTreino), ref(tabelaHashClassesTreino),
                                 totalLinhas, ref(acertos), ref(erros), ref(arquivoSaida)));
    }

    // Aguarda todas as threads terminarem
    for (auto& t : threads) {
        t.join();
    }

    double porcentagemAcertos = (static_cast<double>(acertos) / totalLinhasArquivo) * 100.0;
    double porcentagemErros = (static_cast<double>(erros) / totalLinhasArquivo) * 100.0;

    arquivoSaida << "Total de acertos: " << acertos << " (" << fixed << setprecision(2) << porcentagemAcertos << "%)" << endl;
    arquivoSaida << "Total de erros: " << erros << " (" << fixed << setprecision(2) << porcentagemErros << "%)" << endl;

    arquivoTeste.close();
    arquivoSaida.close();

    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> duracao = fim - inicio;
    cout << "[ Tempo de execução: " << duracao.count() << " segundos ]" << endl;
}