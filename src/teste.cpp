#include "teste.hpp"
#include <chrono>
#include <iomanip>  // Para usar std::setprecision
#include <vector>
#include <map>  // Add this line
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
#define maxComb 3

using namespace std;


std::mutex mutexArquivo;  // Mutex para proteger o acesso ao arquivo de saída
std::mutex mutexContadores;  // Mutex para proteger os contadores de acertos e erros

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
// Define os rankings das mãos de pôquer
enum PokerHand {
    NADA = 0,
    UM_PAR,
    DOIS_PARES,
    TRINCA,
    SEQUENCIA,
    FLUSH,
    FULL_HOUSE,
    QUADRA,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};

// Mutex para proteger o acesso à variável `relevanciaClasse`
// O mutex garante que apenas uma thread por vez possa modificar a `relevanciaClasse`,
// evitando problemas de concorrência (como condições de corrida).
std::mutex mutexRelevancia;

// Função para criar pares de naipe e valor para cada carta
vector<pair<int, int>> criarParesDeCartas(const vector<int>& linhaValores) {
    vector<pair<int, int>> pares;
    
    // Certifica-se de que a entrada contém pelo menos 5 pares de cartas mais a classe
    if (linhaValores.size() < 10) {
        return pares;
    }

    // Adiciona os pares de naipe e valor, ignorando o último valor (classe)
    for (size_t i = 0; i < linhaValores.size() - 1; i += 2) {
        pares.push_back({linhaValores[i], linhaValores[i + 1]});
    }

    return pares;
}


int determinarMaoDePoker(const vector<pair<int, int>>& paresCartas) {
    map<int, int> valorContagem;
    map<int, int> naipeContagem;
    bool isFlush = true;
    bool isStraight = false;

    if (paresCartas.size() != 5) {
        cout << "Número incorreto de cartas." << endl;
        return NADA;
    }

    // Contar a frequência de cada valor e naipe
    for (const auto& par : paresCartas) {
        valorContagem[par.second]++;
        naipeContagem[par.first]++;
    }

    // Verificar se todas as cartas têm o mesmo naipe (Flush)
    if (naipeContagem.size() > 1) {
        isFlush = false;
    }

    // Verificar se as cartas formam uma sequência (Straight)
    vector<int> valoresUnicos;
    for (const auto& par : valorContagem) {
        valoresUnicos.push_back(par.first);
    }
    sort(valoresUnicos.begin(), valoresUnicos.end());

    // Função auxiliar para verificar se a sequência é válida
    auto verificarSequencia = [](const vector<int>& valores) {
        if (valores.size() < 5) return false;
        for (size_t i = 0; i < valores.size() - 4; ++i) {
            bool sequencia = true;
            for (size_t j = 0; j < 4; ++j) {
                if (valores[i + j] != valores[i + j + 1] - 1) {
                    sequencia = false;
                    break;
                }
            }
            if (sequencia) return true;
        }
        return false;
    };

    // Verificar Straight
    if (verificarSequencia(valoresUnicos)) {
        isStraight = true;
    } else {
        // Transformar 1 em 14 e verificar novamente
        vector<int> valoresComAs = valoresUnicos;
        if (find(valoresComAs.begin(), valoresComAs.end(), 1) != valoresComAs.end()) {
            replace(valoresComAs.begin(), valoresComAs.end(), 1, 14);
            sort(valoresComAs.begin(), valoresComAs.end());
            if (verificarSequencia(valoresComAs)) {
                isStraight = true;
            }
        }
    }

    // Verificar Royal Flush especificamente
    if (isFlush && isStraight) {
        // Verificar se é Royal Flush, considerando que o valor 1 pode ser 14
        vector<int> valoresComAsTransformado = valoresUnicos;
        if (find(valoresComAsTransformado.begin(), valoresComAsTransformado.end(), 1) != valoresComAsTransformado.end()) {
            replace(valoresComAsTransformado.begin(), valoresComAsTransformado.end(), 1, 14);
            sort(valoresComAsTransformado.begin(), valoresComAsTransformado.end());
        }

        if (valoresComAsTransformado == vector<int>({10, 11, 12, 13, 14})) {
            return ROYAL_FLUSH;
        } else if (valoresUnicos == vector<int>({10, 11, 12, 13, 14})) {
            return ROYAL_FLUSH;
        }
    }

    // Identificar outras mãos específicas de pôquer
    if (isFlush && isStraight) {
        return STRAIGHT_FLUSH;
    } else if (valorContagem.size() == 2) {
        auto it = valorContagem.begin();
        if (it->second == 4 || (++it)->second == 4) {
            return QUADRA;
        } else {
            return FULL_HOUSE;
        }
    } else if (isFlush) {
        return FLUSH;
    } else if (isStraight) {
        return SEQUENCIA;
    } else if (valorContagem.size() == 3) {
        auto it = valorContagem.begin();
        if (it->second == 3 || (++it)->second == 3) {
            return TRINCA;
        } else {
            return DOIS_PARES;
        }
    } else if (valorContagem.size() == 4) {
        return UM_PAR;
    } else {
        return NADA;
    }
}




// Função para criar buckets com base nas mãos de pôquer e salvar em arquivo
unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> criarBucketsComPokerHands(const string& nomeArquivoTeste, int& totalLinhas) {
    ifstream arquivoTeste(nomeArquivoTeste);
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets;
    vector<pair<vector<int>, int>> linhas;
    string linha;
    totalLinhas = 0;
    int numeroLinha = 1;

    while (getline(arquivoTeste, linha)) {
        stringstream ss(linha);
        string item;
        vector<int> linhaValores;

        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }

        // Cria os pares de naipe e valor
        vector<pair<int, int>> paresCartas = criarParesDeCartas(linhaValores);

        // Determina a mão de pôquer para a linha atual
        int pokerHand = determinarMaoDePoker(paresCartas);

        buckets[pokerHand].first.push_back({linhaValores, numeroLinha});

        totalLinhas++;
        numeroLinha++;
    }

    for (auto& bucket : buckets) {
        int totalLinhasBucket = bucket.second.first.size();
        double suporte = static_cast<double>(totalLinhasBucket) / totalLinhas;
        bucket.second.second = suporte;
    }
    return buckets;
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

<<<<<<< Updated upstream
=======
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

// Função auxiliar para processar um subconjunto de linhas e atribuí-las aos buckets
void processarLinhasParaBuckets(int inicio, int fim, 
                                const vector<pair<vector<int>, int>>& linhas, 
                                unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>& buckets) {
    for (int i = inicio; i < fim; ++i) {
        const auto& linha = linhas[i];
        int melhorBucket = -1;
        double maiorSimilaridade = 0.0;

        for (auto& bucket : buckets) {
            size_t comparacoes = min(bucket.second.first.size(), static_cast<size_t>(10));
            for (size_t j = 0; j < comparacoes; ++j) {
                double similaridade = calcularSimilaridade(linha.first, bucket.second.first[j].first);
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
                    for (size_t j = 0; j < comparacoes; ++j) {
                        somaSimilaridade += calcularSimilaridade(linha.first, bucket.second.first[j].first);
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
}

// Função para criar buckets com base na similaridade
unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> criarBucketsComSimilaridade(
    const vector<string>& linhas, int inicio, int fim, int& totalLinhas, int maxComb,
    const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
    const unordered_map<int, set<int>>& tabelaHashClasses) {

    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets;
    unordered_map<int, double> suportesExistentes; // Armazena suportes já calculados
    vector<pair<vector<int>, int>> linhasProcessadas;
    set<int> classesDistintas;
    totalLinhas = 0;

    for (int i = inicio; i < fim; ++i) {
        stringstream ss(linhas[i]);
        string item;
        vector<int> linhaValores;
        int classe;

        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }

        classe = linhaValores.back();
        classesDistintas.insert(classe);
        linhasProcessadas.push_back({linhaValores, classe});
        totalLinhas++;
    }

    for (int classe : classesDistintas) {
        buckets[classe] = make_pair(vector<pair<vector<int>, int>>(), 0.0);
    }

    for (const auto& linha : linhasProcessadas) {
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

    return buckets;
}

>>>>>>> Stashed changes
// Função para avaliar a classe de uma linha
int avaliarClasse(const vector<int>& linha, 
                  const unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>& buckets,
                  const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                  const unordered_map<int, set<int>>& tabelaHashClasses,
                  int totalLinhas) {
    // Verifica se a linha está em algum bucket
    for (const auto& bucket : buckets) {
        for (const auto& linhaBucket : bucket.second.first) {
            vector<int> linhaBucketCopy = linhaBucket.first;
            linhaBucketCopy.pop_back();
            
            if (linha == linhaBucketCopy) {
                // Linha está em um bucket, retorna a classe associada ao bucket
                return bucket.first;
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
    selecionarLinhasAleatorias(nomeArquivoTeste, "dataset/20linhas.txt", 10000);

    ifstream arquivoTeste("dataset/20linhas.txt");
    auto inicio = chrono::high_resolution_clock::now();

<<<<<<< Updated upstream
    int totalLinhas;
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets = criarBucketsComPokerHands("dataset/20linhas.txt", totalLinhas);

=======
>>>>>>> Stashed changes
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
    vector<unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>> resultadosBuckets(numThreads);

    // Cria threads para criar buckets em subconjuntos de linhas
    for (int i = 0; i < numThreads; ++i) {
        int inicio = i * linhasPorThread;
        int fim = (i == numThreads - 1) ? totalLinhasArquivo : inicio + linhasPorThread;

        threads.push_back(thread(
            [&, i, inicio, fim] {
                int totalLinhas = fim - inicio;
                int maxcomb = 3;  // ou algum valor apropriado
                resultadosBuckets[i] = criarBucketsComSimilaridade(linhas, inicio, fim, totalLinhas, maxcomb, tabelaHashTreino, tabelaHashClassesTreino);
            }
        ));
    }

    // Aguarda todas as threads terminarem
    for (auto& t : threads) {
        t.join();
    }

    // Mescla os resultados dos buckets criados por cada thread
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets;
    for (const auto& resultado : resultadosBuckets) {
        for (const auto& bucket : resultado) {
            if (buckets.find(bucket.first) == buckets.end()) {
                buckets[bucket.first] = bucket.second;
            } else {
                buckets[bucket.first].first.insert(
                    buckets[bucket.first].first.end(),
                    bucket.second.first.begin(),
                    bucket.second.first.end()
                );
                buckets[bucket.first].second += bucket.second.second;
            }
        }
    }

    // Inicia o processamento das linhas usando threads
    threads.clear();
    for (int i = 0; i < numThreads; ++i) {
        int inicio = i * linhasPorThread;
        int fim = (i == numThreads - 1) ? totalLinhasArquivo : inicio + linhasPorThread;

        threads.push_back(thread(processarLinhas, i, ref(linhas), inicio, fim, ref(buckets),
                                 ref(tabelaHashTreino), ref(tabelaHashClassesTreino),
                                 totalLinhasArquivo, ref(acertos), ref(erros), ref(arquivoSaida)));
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
