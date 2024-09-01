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

const double THRESHOLD_SIMILARIDADE = 0.7;
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
// Função que calcula a interseção das linhas e atualiza a relevância das classes para uma combinação
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

    // Loop para considerar combinações de 1 até maxComb features
    for (int interacao = 1; interacao <= maxComb; interacao++) {
        // Gera todas as combinações de `interacao` elementos dos índices
        auto combs = combinacoes(indices, interacao);
        
        // Para cada combinação, chama `calcularSuportePorCombinacao`
        for (const auto& comb : combs) {
            calcularSuportePorCombinacao(linhas, comb, tabelaHashClasses, relevanciaClasse, totalLinhas);
        }
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

// Função para calcular o suporte do bucket
double calcularSuporteBucket(const vector<pair<vector<int>, int>>& bucket,
                             int totalLinhas, int maxComb,
                             const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                             const unordered_map<int, set<int>>& tabelaHashClasses,
                             const unordered_map<int, double>& suportesExistentes) {
    vector<int> classesAtribuidas;
    size_t numLinhas = min(bucket.size(), size_t(5));

    for (size_t j = 0; j < numLinhas; ++j) {
        const vector<int>& linha = bucket[j].first;
        int classe = bucket[j].second;

        if (classe == -1) {
            vector<tuple<int, int>> featuresLinha;
            for (size_t i = 0; i < linha.size(); ++i) {
                featuresLinha.push_back(make_tuple(i + 1, linha[i]));
            }
            classe = avaliarClasseCombinatoria(tabelaHash, tabelaHashClasses, featuresLinha, totalLinhas);
        }
        classesAtribuidas.push_back(classe);
    }

    double somaClasses = 0.0;
    for (int classe : classesAtribuidas) {
        somaClasses += classe;
    }
    double classeMedia = classesAtribuidas.empty() ? 0.0 : somaClasses / classesAtribuidas.size();
    double suporteTotal = (maxComb > 0) ? classeMedia / maxComb : 0.0;

    // Verifica se o suporte calculado é semelhante aos já existentes
    double tolerancia = 0;  // Ajuste a tolerância conforme necessário
    for (const auto& [_, suporteExistente] : suportesExistentes) {
        if (abs(suporteTotal - suporteExistente) < tolerancia) {
            // Se o suporte for muito próximo, ajusta o valor para ser distinto
            suporteTotal += tolerancia;
            //tolerancia+=1;
        }
    }

    return suporteTotal;
}

// Função principal para criar buckets com base na similaridade
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

    for (int classe : classesDistintas) {
        buckets[classe] = make_pair(vector<pair<vector<int>, int>>(), 0.0);
    }

    for (const auto& linha : linhas) {
        int melhorBucket = -1;
        double maiorSimilaridade = 0.0;

        for (auto& bucket : buckets) {
            size_t comparacoes = min(bucket.second.first.size(), static_cast<size_t>(10));
            for (size_t i = 0; i < comparacoes; ++i) {
                double similaridade = calcularSimilaridade(linha.first, bucket.second.first[i].first);
                if (similaridade > maiorSimilaridade) {
                    maiorSimilaridade = similaridade;
                    melhorBucket = bucket.first;
                }
            }
        }

        if (maiorSimilaridade > 0.1) {
            buckets[melhorBucket].first.push_back({linha.first, linha.second});
        } else {
            bool adicionado = false;
            for (auto& bucket : buckets) {
                if (bucket.second.first.empty()) {
                    bucket.second.first.push_back({linha.first, linha.second});
                    adicionado = true;
                    break;
                }
            }

            if (!adicionado) {
                double maiorMediaSimilaridade = 0.0;
                int melhorBucketParaAdicionar = -1;

                for (const auto& bucket : buckets) {
                    double somaSimilaridade = 0.0;
                    int count = 0;
                    size_t comparacoes = min(bucket.second.first.size(), static_cast<size_t>(10));
                    for (size_t i = 0; i < comparacoes; ++i) {
                        somaSimilaridade += calcularSimilaridade(linha.first, bucket.second.first[i].first);
                        count++;
                    }
                    double mediaSimilaridade = (count > 0) ? somaSimilaridade / count : 0.0;
                    if (mediaSimilaridade > maiorMediaSimilaridade) {
                        maiorMediaSimilaridade = mediaSimilaridade;
                        melhorBucketParaAdicionar = bucket.first;
                    }
                }

                if (melhorBucketParaAdicionar != -1) {
                    buckets[melhorBucketParaAdicionar].first.push_back({linha.first, linha.second});
                }
            }
        }
    }

    for (auto& bucket : buckets) {
        double suporte = calcularSuporteBucket(bucket.second.first, totalLinhas, maxComb, tabelaHash, tabelaHashClasses, suportesExistentes);
        bucket.second.second = suporte;
        suportesExistentes[bucket.first] = suporte;
    }

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
    }

    return buckets;
}

// Função para avaliar a classe de uma linha
int avaliarClasse(const vector<int>& linha, 
                  const unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>& buckets,
                  const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                  const unordered_map<int, set<int>>& tabelaHashClasses,
                  int totalLinhas) {
    cout << "Iniciando avaliação da linha: ";
    for (int valor : linha) {
        cout << valor << " ";
    }
    cout << endl;

    // Verifica se a linha está em algum bucket
    for (const auto& bucket : buckets) {
        cout << "Verificando bucket " << bucket.first << " com suporte " << bucket.second.second << endl;

        // Itera sobre as linhas no bucket
        for (const auto& linhaBucket : bucket.second.first) {
            // Cria uma cópia da linha do bucket
            vector<int> linhaBucketCopy = linhaBucket.first;
            linhaBucketCopy.pop_back();
            
            // Se a linha do bucket e a linha a ser comparada forem iguais
            cout << "Comparando com linha do bucket: ";
            for (int valor : linhaBucketCopy) {
                cout << valor << " ";
            }
            cout << endl;
            
            if (linha == linhaBucketCopy) {
                // Linha está em um bucket, retorna a classe associada ao bucket
                cout << "Linha encontrada no bucket " << bucket.first << endl;
                cout << "Classe atribuída: " << bucket.second.second << endl;
                return bucket.second.second;
            }
        }
    }

    // Se a linha não estiver em um bucket, prossegue com a análise combinatória
    cout << "Linha não encontrada em nenhum bucket. Iniciando análise combinatória." << endl;
    vector<tuple<int, int>> featuresLinha;
    for (size_t i = 0; i < linha.size(); ++i) {
        featuresLinha.push_back(make_tuple(i + 1, linha[i])); // Ajusta para a estrutura de tupla (coluna, valor)
    }
    
    // Avalia a classe usando a análise combinatória
    int classeAtribuida = avaliarClasseCombinatoria(tabelaHash, tabelaHashClasses, featuresLinha, totalLinhas);
    cout << "Classe atribuída após análise combinatória: " << classeAtribuida << endl;

    return classeAtribuida;
}

// Função auxiliar para processar um subconjunto de linhas
void processarLinhas(int threadId, vector<string>& linhas, int inicio, int fim,
                     const unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>& buckets,
                     const unordered_map<tuple<int, int>, set<int>>& tabelaHashTreino,
                     const unordered_map<int, set<int>>& tabelaHashClassesTreino,
                     int totalLinhas, int& acertos, int& erros, ofstream& arquivoSaida) {
    // Loop que percorre um subconjunto de linhas de acordo com o intervalo [inicio, fim]
    for (int i = inicio; i < fim; ++i) {
        // Mensagem de depuração (não faz nada com threadId atualmente, mas pode ser usado para depuração)
        threadId = threadId;

        // Cria um stringstream a partir da linha do vetor de linhas
        stringstream ss(linhas[i]);
        string item;
        vector<int> linhaValores;

        // Lê os valores separados por vírgulas na linha e os armazena no vetor linhaValores
        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }

        // Verifica se o vetor linhaValores não está vazio
        if (!linhaValores.empty()) {
            // A última posição do vetor linhaValores contém a classe original
            int classeOriginal = linhaValores.back();
            linhaValores.pop_back(); // Remove a classe original dos valores para processar apenas os dados

            // Avalia a classe da linha com base nos dados e buckets fornecidos
            int classeAtribuida = avaliarClasse(linhaValores, buckets, tabelaHashTreino, tabelaHashClassesTreino, totalLinhas);

            // Bloqueia o mutex para garantir acesso exclusivo ao arquivo de saída
            {
                lock_guard<mutex> lock(mutexArquivo);
                // Escreve o resultado da linha processada no arquivo de saída
                arquivoSaida << "Linha " << (i + 1) << ": Classe Atribuída = " << classeAtribuida << endl;
            }

            // Bloqueia o mutex para garantir acesso exclusivo aos contadores de acertos e erros
            {
                lock_guard<mutex> lock(mutexContadores);
                // Verifica se a classe atribuída corresponde à classe original
                if (classeAtribuida == classeOriginal) {
                    acertos++; // Incrementa o contador de acertos se a classe estiver correta
                } else {
                    erros++; // Incrementa o contador de erros se a classe estiver incorreta
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
    int maxcomb = 3;
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets = criarBucketsComSimilaridade(nomeArquivoTeste, totalLinhas, maxcomb, tabelaHashTreino, tabelaHashClassesTreino);

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