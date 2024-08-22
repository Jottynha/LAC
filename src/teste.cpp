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
        cout << "Número insuficiente de valores para formar cartas." << endl;
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

    // Exibir a contagem de valores e naipes
    cout << "Contagem de valores:" << endl;
    for (const auto& [valor, contagem] : valorContagem) {
        cout << "Valor: " << valor << ", Contagem: " << contagem << endl;
    }

    cout << "Contagem de naipes:" << endl;
    for (const auto& [naipe, contagem] : naipeContagem) {
        cout << "Naipe: " << naipe << ", Contagem: " << contagem << endl;
    }

    // Verificar se todas as cartas têm o mesmo naipe (Flush)
    if (naipeContagem.size() > 1) {
        isFlush = false;
    }
    cout << "É Flush: " << (isFlush ? "Sim" : "Não") << endl;

    // Verificar se as cartas formam uma sequência (Straight)
    vector<int> valoresUnicos;
    for (const auto& par : valorContagem) {
        valoresUnicos.push_back(par.first);
    }
    sort(valoresUnicos.begin(), valoresUnicos.end());

    cout << "Valores únicos ordenados:" << endl;
    for (const auto& valor : valoresUnicos) {
        cout << valor << " ";
    }
    cout << endl;

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

    cout << "É Sequência: " << (isStraight ? "Sim" : "Não") << endl;

    // Verificar Royal Flush especificamente
    if (isFlush && isStraight) {
        // Verificar se é Royal Flush, considerando que o valor 1 pode ser 14
        vector<int> valoresComAsTransformado = valoresUnicos;
        if (find(valoresComAsTransformado.begin(), valoresComAsTransformado.end(), 1) != valoresComAsTransformado.end()) {
            replace(valoresComAsTransformado.begin(), valoresComAsTransformado.end(), 1, 14);
            sort(valoresComAsTransformado.begin(), valoresComAsTransformado.end());
        }

        if (valoresComAsTransformado == vector<int>({10, 11, 12, 13, 14})) {
            cout << "Mão: Royal Flush" << endl;
            return ROYAL_FLUSH;
        } else if (valoresUnicos == vector<int>({10, 11, 12, 13, 14})) {
            cout << "Mão: Royal Flush" << endl;
            return ROYAL_FLUSH;
        }
    }

    // Identificar outras mãos específicas de pôquer
    if (isFlush && isStraight) {
        cout << "Mão: Straight Flush" << endl;
        return STRAIGHT_FLUSH;
    } else if (valorContagem.size() == 2) {
        auto it = valorContagem.begin();
        if (it->second == 4 || (++it)->second == 4) {
            cout << "Mão: Quadra" << endl;
            return QUADRA;
        } else {
            cout << "Mão: Full House" << endl;
            return FULL_HOUSE;
        }
    } else if (isFlush) {
        cout << "Mão: Flush" << endl;
        return FLUSH;
    } else if (isStraight) {
        cout << "Mão: Sequência" << endl;
        return SEQUENCIA;
    } else if (valorContagem.size() == 3) {
        auto it = valorContagem.begin();
        if (it->second == 3 || (++it)->second == 3) {
            cout << "Mão: Trinca" << endl;
            return TRINCA;
        } else {
            cout << "Mão: Dois Pares" << endl;
            return DOIS_PARES;
        }
    } else if (valorContagem.size() == 4) {
        cout << "Mão: Um Par" << endl;
        return UM_PAR;
    } else {
        cout << "Mão: Nada" << endl;
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

    // Salva os buckets em um arquivo externo
    ofstream arquivoBuckets("dataset/buckets.txt");
    if (arquivoBuckets.is_open()) {
        for (const auto& bucket : buckets) {
            arquivoBuckets << "Bucket " << bucket.first << " (" << bucket.first << "):" << endl;
            for (const auto& linha : bucket.second.first) {
                for (size_t i = 0; i < linha.first.size(); ++i) {
                    arquivoBuckets << linha.first[i] << (i < linha.first.size() - 1 ? "," : "");
                }
                arquivoBuckets << " [Linha: " << linha.second << "]" << endl;
            }
            arquivoBuckets << "Suporte: " << bucket.second.second << endl << endl;
        }
        arquivoBuckets.close();
    } else {
        cerr << "Erro ao abrir o arquivo buckets.txt" << endl;
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
    selecionarLinhasAleatorias(nomeArquivoTeste,"dataset/20linhas.txt",20);
    ifstream arquivoTeste("dataset/20linhas.txt");
    auto inicio = chrono::high_resolution_clock::now();
    
    int totalLinhas;
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets = criarBucketsComPokerHands("dataset/20linhas.txt", totalLinhas);

    // Abre o arquivo de teste para leitura
    //ifstream arquivoTeste(nomeArquivoTeste);
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

    // Lê o arquivo de teste linha por linha
    string linha;
    while (getline(arquivoTeste, linha)) {
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
