#include "teste.hpp"

set<int> intersecao(const set<int>& set1, const set<int>& set2) {
    set<int> resultado;
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(resultado, resultado.begin()));
    return resultado;
}


template <typename T>
vector<vector<T>> gerarCombinacoes(const vector<T>& elementos, int tamanho) {
    vector<vector<T>> combinacoes;
    vector<int> combinacao(tamanho);
    iota(combinacao.begin(), combinacao.end(), 0); // Preenche com [0, 1, ..., tamanho-1]

    do {
        vector<T> novaCombinacao;
        for (int i : combinacao) {
            novaCombinacao.push_back(elementos[i]);
        }
        combinacoes.push_back(novaCombinacao);
    } while (next_permutation(combinacao.begin(), combinacao.end()));

    return combinacoes;
}

template <typename Iterator>
bool next_combination(Iterator first, Iterator last, size_t n) {
    size_t k = std::distance(first, last);
    if (k == 0 || k > n) return false;

    for (size_t i = k; i-- > 0;) {
        if (*(first + i) < n - (k - i) ) {
            ++(*(first + i));
            for (size_t j = i + 1; j < k; ++j) {
                *(first + j) = *(first + j - 1) + 1;
            }
            return true;
        }
    }
    return false;
}
int avaliarCombinacoes(const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                       const unordered_map<int, set<int>>& tabelaHashClasses,
                       const vector<tuple<int, int>>& featuresLinha) {
    set<int> linhasCombinadas;
    vector<size_t> combinacao(featuresLinha.size());
    iota(combinacao.begin(), combinacao.end(), 0);

    // Avaliar combinações de tamanho 1 até o tamanho total
    for (size_t k = 1; k <= featuresLinha.size(); ++k) {
        do {
            set<int> linhasIntersecao;
            for (size_t i = 0; i < k; ++i) {
                const auto& feature = featuresLinha[combinacao[i]];
                auto it = tabelaHash.find(feature);
                if (it != tabelaHash.end()) {
                    const set<int>& linhasFeature = it->second;
                    if (i == 0) {
                        linhasIntersecao = linhasFeature;
                    } else {
                        linhasIntersecao = intersecao(linhasIntersecao, linhasFeature);
                    }
                }
            }

            if (!linhasIntersecao.empty()) {
                // Encontrar a classe com a maior interseção
                int classeMaxima = -1;
                size_t maxCount = 0;
                for (const auto& classe : tabelaHashClasses) {
                    set<int> linhasClasse = intersecao(linhasIntersecao, classe.second);
                    if (linhasClasse.size() > maxCount) {
                        maxCount = linhasClasse.size();
                        classeMaxima = classe.first;
                    }
                }

                if (classeMaxima != -1) {
                    return classeMaxima;
                }
            }
        } while (next_combination(combinacao.begin(), combinacao.end(), featuresLinha.size()));
    }

    // Se nenhuma classe foi determinada, retornar uma classe padrão ou erro
    return -1;
}

void processarLinha(const vector<tuple<int, int>>& tuplasLinha, const unordered_map<tuple<int, int>, set<int>>& tabelaHash, const unordered_map<int, set<int>>& tabelaHashClasses, int totalLinhas) {
    set<int> linhasCombinadas;
    bool primeiraFeature = true;

    for (const auto& tupla : tuplasLinha) {
        int coluna = get<0>(tupla);
        int valor = get<1>(tupla);

        set<int> linhasFeature = buscarFeature(tabelaHash, coluna, valor);

        if (primeiraFeature) {
            linhasCombinadas = linhasFeature;
            primeiraFeature = false;
        } else {
            linhasCombinadas = intersecao(linhasCombinadas, linhasFeature);
        }
    }

    unordered_map<int, double> relevanciaClasse;

    for (const auto& classe : tabelaHashClasses) {
        set<int> linhasClasse = classe.second;
        set<int> linhasIntersecao = intersecao(linhasCombinadas, linhasClasse);

        if (!linhasIntersecao.empty()) {
            double suporte = static_cast<double>(linhasIntersecao.size()) / totalLinhas;
            relevanciaClasse[classe.first] += suporte;
        }
    }

    vector<pair<int, double>> suporteClasses(relevanciaClasse.begin(), relevanciaClasse.end());
    sort(suporteClasses.begin(), suporteClasses.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });

    if (!suporteClasses.empty()) {
        int classeDefinida = suporteClasses.front().first;
        cout << "Classe definida para a entrada: " << classeDefinida << endl;
    } else {
        cout << "Nenhuma classe encontrada." << endl;
    }
}

void teste(const string& nomeArquivoTeste) {
    ifstream arquivoTeste(nomeArquivoTeste);
    ofstream arquivoSaida("dataset/output.txt");
    vector<vector<tuple<int, int>>> tuplasTeste;
    vector<int> classesTeste;
    string linha;
    int totalLinhas = 0;
    int acertos = 0;
    int erros = 0;

    if (!arquivoTeste) {
        cerr << "Erro ao abrir o arquivo de teste." << endl;
        return;
    }

    while (getline(arquivoTeste, linha)) {
        vector<tuple<int, int>> linhaTuplas;
        stringstream ss(linha);
        string item;
        vector<int> linhaValores;

        while (getline(ss, item, ',')) {
            linhaValores.push_back(stoi(item));
        }

        if (!linhaValores.empty()) {
            int classe = linhaValores.back();
            classesTeste.push_back(classe);
            linhaValores.pop_back();

            for (size_t i = 0; i < linhaValores.size(); ++i) {
                linhaTuplas.push_back(make_tuple(i + 1, linhaValores[i]));
            }

            tuplasTeste.push_back(linhaTuplas);

            // Avaliação da combinação
            int classeAtribuida = avaliarCombinacoes(tabelaHashTreino, tabelaHashClassesTreino, linhaTuplas);

            // Escrever no arquivo de saída
            arquivoSaida << "Linha " << (totalLinhas + 1) << ": Classe Atribuída = " << classeAtribuida << endl;

            // Comparar com a classe original
            if (classeAtribuida == classesTeste.back()) {
                acertos++;
            } else {
                erros++;
            }

            totalLinhas++;
        }
    }

    // Escrever o número de acertos e erros no arquivo de saída
    arquivoSaida << "Total de acertos: " << acertos << endl;
    arquivoSaida << "Total de erros: " << erros << endl;

    arquivoTeste.close();
    arquivoSaida.close();
}
