#include "teste.hpp"
#include <chrono>
#include <iomanip>  // Para usar std::setprecision

template <typename T>
vector<vector<T>> combinacoes(const vector<T>& iterable, int r) {
    vector<vector<T>> resultado;
    int n = iterable.size();
    if (r > n) return resultado; // Se r for maior que o tamanho do vetor, retorna vazio
    vector<int> indices(r);
    for (int i = 0; i < r; ++i) {
        indices[i] = i;
    }
    resultado.push_back({});
    for (int i = 0; i < r; ++i) {
        resultado.back().push_back(iterable[indices[i]]);
    }
    while (true) {
        int i;
        for (i = r - 1; i >= 0; --i) {
            if (indices[i] != i + n - r) {
                break;
            }
        }
        if (i < 0) {
            return resultado;
        }
        ++indices[i];
        for (int j = i + 1; j < r; ++j) {
            indices[j] = indices[j - 1] + 1;
        }
        resultado.push_back({});
        for (int i = 0; i < r; ++i) {
            resultado.back().push_back(iterable[indices[i]]);
        }
    }
}

// Função para avaliar a classe de uma linha de features
int avaliarClasse(const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                  const unordered_map<int, set<int>>& tabelaHashClasses,
                  const vector<tuple<int, int>>& featuresLinha, int totalLinhas) {
    // Mapa para armazenar a relevância de cada classe
    unordered_map<int, double> relevanciaClasse;
    // Vetor para armazenar os conjuntos de linhas para cada feature
    vector<set<int>> linhas;

    // Itera sobre cada feature da linha
    for (const auto& tupla : featuresLinha) {
        int coluna = get<0>(tupla);
        int valor = get<1>(tupla);
        // Busca o conjunto de linhas para a feature na tabela hash
        set<int> linhasFeature = buscarFeature(tabelaHash, coluna, valor);
        if (!linhasFeature.empty()) { // Verifica se o conjunto de linhas não está vazio
            linhas.push_back(linhasFeature);
        }
    }

    int numLinhas = linhas.size();
    vector<int> indices(numLinhas);
    iota(indices.begin(), indices.end(), 0);
    
    // Iterações para encontrar interseções de linhas e calcular relevância
    for (int interacao = 1; interacao < 4; interacao++) {
        switch (interacao) {
            case 1: {
                // Primeira interação: calcula relevância para interseções simples
                for (const auto& linhasFeature : linhas) {
                    for (const auto& classe : tabelaHashClasses) {
                        set<int> linhasClasse = classe.second;
                        set<int> linhasIntersecao;
                        // Calcula a interseção entre linhas da feature e linhas da classe
                        set_intersection(linhasFeature.begin(), linhasFeature.end(),
                                         linhasClasse.begin(), linhasClasse.end(),
                                         inserter(linhasIntersecao, linhasIntersecao.begin()));
                        if (!linhasIntersecao.empty()) {
                            // Calcula o suporte e atualiza a relevância da classe
                            double suporte = static_cast<double>(linhasIntersecao.size()) / totalLinhas;
                            relevanciaClasse[classe.first] += suporte;
                        }
                    }
                }
                break;
            }
            case 2: {
                // Segunda interação: calcula relevância para interseções duplas
                auto combs = combinacoes(indices, 2);
                for (const auto& comb : combs) {
                    int i = comb[0];
                    int j = comb[1];
                    
                    set<int> intersecao12;
                    // Calcula a interseção entre duas features
                    set_intersection(linhas[i].begin(), linhas[i].end(),
                                          linhas[j].begin(), linhas[j].end(),
                                          std::inserter(intersecao12, intersecao12.begin()));

                    for (const auto& classe : tabelaHashClasses) {
                        set<int> linhasClasse = classe.second;
                        set<int> linhasIntersecao;
                        // Calcula a interseção entre a interseção dupla e linhas da classe
                        set_intersection(intersecao12.begin(), intersecao12.end(),
                                              linhasClasse.begin(), linhasClasse.end(),
                                              std::inserter(linhasIntersecao, linhasIntersecao.begin()));
                        if (!linhasIntersecao.empty()) {
                            // Calcula o suporte e atualiza a relevância da classe
                            double suporte = static_cast<double>(linhasIntersecao.size()) / totalLinhas;
                            relevanciaClasse[classe.first] += suporte;
                        }
                    }
                }
                break;
            }
            case 3: {
                // Terceira interação: calcula relevância para interseções triplas
                auto combs = combinacoes(indices, 3);
                for (const auto& comb : combs) {
                    int i = comb[0];
                    int j = comb[1];
                    int k = comb[2];

                    std::set<int> intersecao12;
                    // Calcula a interseção entre duas features
                    std::set_intersection(linhas[i].begin(), linhas[i].end(),
                                          linhas[j].begin(), linhas[j].end(),
                                          std::inserter(intersecao12, intersecao12.begin()));

                    std::set<int> intersecao123;
                    // Calcula a interseção entre três features
                    std::set_intersection(intersecao12.begin(), intersecao12.end(),
                                          linhas[k].begin(), linhas[k].end(),
                                          std::inserter(intersecao123, intersecao123.begin()));

                    for (const auto& classe : tabelaHashClasses) {
                        std::set<int> linhasClasse = classe.second;
                        std::set<int> linhasIntersecao;
                        // Calcula a interseção entre a interseção tripla e linhas da classe
                        std::set_intersection(intersecao123.begin(), intersecao123.end(),
                                              linhasClasse.begin(), linhasClasse.end(),
                                              std::inserter(linhasIntersecao, linhasIntersecao.begin()));
                        if (!linhasIntersecao.empty()) {
                            // Calcula o suporte e atualiza a relevância da classe
                            double suporte = static_cast<double>(linhasIntersecao.size()) / totalLinhas;
                            relevanciaClasse[classe.first] += suporte;
                        }
                    }
                }
                break;
            }
        }
    }

    // Vetor de pares para armazenar classes e seus suportes, e ordenar por suporte
    vector<pair<int, double>> suporteClasses(relevanciaClasse.begin(), relevanciaClasse.end());
    sort(suporteClasses.begin(), suporteClasses.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });

    if (!suporteClasses.empty()) {
        // Retorna a classe com maior suporte
        return suporteClasses.front().first;
    } else {
        return -1; // Nenhuma classe encontrada
    }
}

// Função para testar o algoritmo com um arquivo de teste
void teste(const string& nomeArquivoTeste) {
    auto inicio = chrono::high_resolution_clock::now();
    // Abre o arquivo de teste para leitura
    ifstream arquivoTeste(nomeArquivoTeste);
    // Abre o arquivo de saída para escrita
    ofstream arquivoSaida("dataset/output.txt");

    // Vetor para armazenar as classes originais do arquivo de teste
    vector<int> classesTeste;
    // Variável para armazenar cada linha do arquivo de teste
    string linha;
    // Variáveis para contar o total de linhas, acertos e erros
    int totalLinhasArquivo = 0;
    int acertos = 0;
    int erros = 0;

    // Verifica se o arquivo de teste foi aberto corretamente
    if (!arquivoTeste) {
        cerr << "Erro ao abrir o arquivo de teste." << endl;
        return;
    }

    // Lê o arquivo de teste linha por linha
    while (getline(arquivoTeste, linha)) {
        // String stream para processar a linha lida
        stringstream ss(linha);
        // Variável para armazenar cada valor separado por vírgula
        string item;
        // Vetor para armazenar os valores inteiros da linha
        vector<int> linhaValores;
        // Reserva espaço no vetor para evitar realocações dinâmicas frequentes
        linhaValores.reserve(1);

        // Separa os valores da linha usando a vírgula como delimitador
        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }

        // Verifica se a linha não está vazia
        if (!linhaValores.empty()) {
            // Extrai a classe original (último valor da linha)
            int classeOriginal = linhaValores.back();
            // Remove o último valor do vetor, que é a classe original
            linhaValores.pop_back();

            // Vetor de tuplas para armazenar os pares (coluna, valor)
            vector<tuple<int, int>> linhaTuplas;
            // Reserva espaço no vetor para evitar realocações dinâmicas frequentes
            linhaTuplas.reserve(linhaValores.size());

            // Cria tuplas de coluna e valor para as features
            for (size_t i = 0; i < linhaValores.size(); ++i) {
                linhaTuplas.emplace_back(i + 1, linhaValores[i]);
            }

            // Avalia a classe atribuída para a linha usando a função avaliarClasse
            int classeAtribuida = avaliarClasse(tabelaHashTreino, tabelaHashClassesTreino, linhaTuplas, totalLinhas);

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
    std::cout << "[ Tempo de execução: " << duracao.count() << " segundos ]" << endl;
}