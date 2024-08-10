#include "teste.hpp"
#include <chrono>
#include <iomanip>  // Para usar std::setprecision

const double THRESHOLD_SIMILARIDADE = 0.7;

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
        cout << "Bucket " << bucket.first << " (Suporte: " << bucket.second.second << "):" << endl;
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

int avaliarClasse(const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                  const unordered_map<int, set<int>>& tabelaHashClasses,
                  const vector<tuple<int, int>>& featuresLinha, int totalLinhas) {
    unordered_map<int, double> relevanciaClasse;
    vector<set<int>> linhas;
    ofstream arquivoSuporte;
    arquivoSuporte.open("dataset/suporte.txt", ios::app);

    for (const auto& tupla : featuresLinha) {
        int coluna = get<0>(tupla);
        int valor = get<1>(tupla);
        set<int> linhasFeature = buscarFeature(tabelaHash, coluna, valor);
        if (!linhasFeature.empty()) {
            linhas.push_back(linhasFeature);
        }
    }

    int numLinhas = linhas.size();
    vector<int> indices(numLinhas);
    iota(indices.begin(), indices.end(), 0);

    for (int interacao = 1; interacao <= 3; interacao++) {
        auto combs = combinacoes(indices, interacao);
        for (const auto& comb : combs) {
            set<int> intersecao = linhas[comb[0]];
            for (long unsigned int idx = 1; idx < comb.size(); ++idx) {
                set<int> tempIntersecao;
                set_intersection(intersecao.begin(), intersecao.end(),
                                 linhas[comb[idx]].begin(), linhas[comb[idx]].end(),
                                 inserter(tempIntersecao, tempIntersecao.begin()));
                intersecao = tempIntersecao;
            }

            for (const auto& classe : tabelaHashClasses) {
                set<int> linhasClasse = classe.second;
                set<int> linhasIntersecao;
                set_intersection(intersecao.begin(), intersecao.end(),
                                 linhasClasse.begin(), linhasClasse.end(),
                                 inserter(linhasIntersecao, linhasIntersecao.begin()));
                if (!linhasIntersecao.empty()) {
                    double suporte = static_cast<double>(linhasIntersecao.size()) / totalLinhas;
                    relevanciaClasse[classe.first] += suporte;
                }
            }
        }
    }

    vector<pair<int, double>> suporteClasses(relevanciaClasse.begin(), relevanciaClasse.end());
    for(long unsigned int a=0;a<suporteClasses.size();a++)
    {
        arquivoSuporte << " Classe: " << get<0>(suporteClasses[a]);     
        arquivoSuporte << " Valor: "<< get<1>(suporteClasses[a]);      
    }
    arquivoSuporte << endl;
    sort(suporteClasses.begin(), suporteClasses.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });

    if (!suporteClasses.empty()) {
        return suporteClasses.front().first;
    } else {
        return -1;
    }
}
// Função para testar o algoritmo com um arquivo de teste
void teste(const string& nomeArquivoTeste) {
    unordered_map<int, pair<vector<pair<vector<int>, int>>, double>> buckets = criarBucketsComSimilaridade(nomeArquivoTeste, totalLinhas);
    ofstream arquivoSuporte("dataset/suporte.txt");
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
    int i = 0;
    while (i<100 && getline(arquivoTeste, linha)) {
        i++;
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
    cout << "[ Tempo de execução: " << duracao.count() << " segundos ]" << endl;
}