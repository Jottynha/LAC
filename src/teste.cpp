#include "teste.hpp"
#include <chrono>
#include <iomanip>  // Para usar std::setprecision
#include <vector>
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

// Mutex para proteger o acesso à variável `relevanciaClasse`
// O mutex garante que apenas uma thread por vez possa modificar a `relevanciaClasse`,
// evitando problemas de concorrência (como condições de corrida).
std::mutex mutexRelevancia;

// Função para calcular similaridade (simples exemplo usando a interseção sobre a união)
double calcularSimilaridade(const vector<int>& linha1, const vector<int>& linha2) {
    int intersecao = 0;
    int uniao = linha1.size();

    for (size_t i = 0; i < linha1.size(); ++i) {
        if (linha1[i] == linha2[i]) {
            intersecao++;
        }
    }

    return static_cast<double>(intersecao) / uniao;
}

// Função para criar buckets com base na similaridade
unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> criarBucketsComSimilaridade(const string& nomeArquivoTeste, int& totalLinhas) {
    ifstream arquivoTeste(nomeArquivoTeste);
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets;
    vector<pair<vector<int>, int>> linhas; // Armazena linhas e números das linhas
    string linha;
    int bucketIndex = 0;
    totalLinhas = 0;
    int numeroLinha = 1;
    int a=0;

    // Lê o arquivo de teste e armazena as linhas com seus números
    while (a<100&&getline(arquivoTeste, linha)) {
        a++;
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
    for (size_t i = 0; i < linhas.size(); ++i) {
        bool adicionado = false;
        for (auto& bucket : buckets) {
            for (const auto& linhaBucket : bucket.second.first) {
                if (calcularSimilaridade(linhas[i].first, linhaBucket.first) >= THRESHOLD_SIMILARIDADE) {
                    bucket.second.first.push_back(linhas[i]);
                    adicionado = true;
                    break;
                }
            }
            if (adicionado) break;
        }

        if (!adicionado) {
            buckets[bucketIndex] = make_pair(vector<pair<vector<int>, int>>{linhas[i]}, 0.0);
            bucketIndex++;
        }
    }

    // Calcula o suporte para cada bucket
    for (auto& bucket : buckets) {
        int totalLinhasBucket = bucket.second.first.size();
        double suporte = static_cast<double>(totalLinhasBucket) / totalLinhas;
        bucket.second.second = suporte;
    }

    // Imprime os buckets e as linhas pertencentes a eles
    cout << "Buckets e linhas pertencentes a cada um:" << endl;
    for (const auto& bucket : buckets) {
        cout << "Bucket " << bucket.first << endl;
        for (const auto& linha : bucket.second.first) {
            for (size_t i = 0; i < linha.first.size(); ++i) {
                cout << linha.first[i] << (i < linha.first.size() - 1 ? "," : "");
            }
            cout << " [Linha: " << linha.second << "]" << endl;
        }
        cout << endl;
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
    ofstream arquivoSuporte("dataset/suporte.txt", ios::app); // Arquivo para armazenar os suportes calculados

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
    for (int interacao = 1; interacao <= 3; interacao++) {
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

    // Armazena no arquivo os valores de suporte calculados para cada classe
    for (const auto& classe : suporteClasses) {
        arquivoSuporte << "Classe: " << classe.first << " Valor: " << classe.second;
    }
    arquivoSuporte.close(); // Fecha o arquivo após escrever os dados

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

// Função para avaliar a classe de uma linha
int avaliarClasse(const vector<int>& linha, 
                  const unordered_map<int, pair<vector<pair<vector<int>, int>>, double>>& buckets,
                  const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                  const unordered_map<int, set<int>>& tabelaHashClasses,
                  int totalLinhas) {
    // Verifica se a linha está em algum bucket
    for (const auto& bucket : buckets) {
        for (const auto& linhaBucket : bucket.second.first) {
            if (linha == linhaBucket.first) {
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

// Função para testar o algoritmo com um arquivo de teste
void teste(const string& nomeArquivoTeste) {
    int totalLinhas;
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets = criarBucketsComSimilaridade(nomeArquivoTeste, totalLinhas);

    // Abre o arquivo de teste para leitura
    ifstream arquivoTeste(nomeArquivoTeste);
    if (!arquivoTeste) {
        cerr << "Erro ao abrir o arquivo de teste." << endl;
        return;
    }

    // Abre o arquivo de saída para escrita
    ofstream arquivoSaida("dataset/output.txt");
    if (!arquivoSaida) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        return;
    }

    // Variáveis para contagem de acertos e erros
    int totalLinhasArquivo = 0;
    int acertos = 0;
    int erros = 0;
    int a=0;

    auto inicio = chrono::high_resolution_clock::now();

    // Lê o arquivo de teste linha por linha
    string linha;
    while (a<100&&getline(arquivoTeste, linha)) {
        a++;
        // String stream para processar a linha lida
        stringstream ss(linha);
        string item;
        vector<int> linhaValores;
        linhaValores.reserve(1);

        // Separa os valores da linha usando a vírgula como delimitador
        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }

        // Verifica se a linha não está vazia
        if (!linhaValores.empty()) {
            // Extrai a classe original (último valor da linha)
            int classeOriginal = linhaValores.back();
            linhaValores.pop_back();

            // Vetor de tuplas para armazenar os pares (coluna, valor)
            vector<tuple<int, int>> linhaTuplas;
            linhaTuplas.reserve(linhaValores.size());

            // Cria tuplas de coluna e valor para as features
            for (size_t i = 0; i < linhaValores.size(); ++i) {
                linhaTuplas.emplace_back(i + 1, linhaValores[i]);
            }

            // Avalia a classe atribuída para a linha usando a função avaliarClasse
            int classeAtribuida = avaliarClasse(linhaValores, buckets, tabelaHashTreino, tabelaHashClassesTreino, totalLinhas);

            // Escreve a classe atribuída no arquivo de saída
            arquivoSaida << "Linha " << (totalLinhasArquivo + 1) << ": Classe Atribuída = " << classeAtribuida << endl;

            // Compara a classe atribuída com a classe original para contar acertos e erros
            if (classeAtribuida == classeOriginal) {
                acertos++;
            } else {
                erros++;
            }

            // Incrementa o contador de linhas processadas
            totalLinhasArquivo++;
        }
    }

    // Calcula as porcentagens de acertos e erros
    double porcentagemAcertos = (static_cast<double>(acertos) / totalLinhasArquivo) * 100.0;
    double porcentagemErros = (static_cast<double>(erros) / totalLinhasArquivo) * 100.0;

    // Escreve o total de acertos e erros no arquivo de saída
    arquivoSaida << "Total de acertos: " << acertos << " (" << fixed << setprecision(2) << porcentagemAcertos << "%)" << endl;
    arquivoSaida << "Total de erros: " << erros << " (" << fixed << setprecision(2) << porcentagemErros << "%)" << endl;

    // Fecha os arquivos de teste e de saída
    arquivoTeste.close();
    arquivoSaida.close();

    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> duracao = fim - inicio;
    cout << "[ Tempo de execução: " << duracao.count() << " segundos ]" << endl;
}
