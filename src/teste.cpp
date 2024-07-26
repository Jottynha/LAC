#include "teste.hpp"

int avaliarClasse(const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                  const unordered_map<int, set<int>>& tabelaHashClasses,
                  const vector<tuple<int, int>>& featuresLinha, int totalLinhas) {
    unordered_map<int, double> relevanciaClasse;
    vector<set<int>> linhas;
    for(int interacao=1;interacao<4;interacao++){
        if(interacao==1){
            for (const auto& tupla : featuresLinha) {
            int coluna = get<0>(tupla);
            int valor = get<1>(tupla);
            set<int> linhasFeature = buscarFeature(tabelaHash, coluna, valor); 
            linhas.push_back(linhasFeature);
            for (const auto& classe : tabelaHashClasses) {
                set<int> linhasClasse = classe.second;
                set<int> linhasIntersecao;
                set_intersection(linhasFeature.begin(), linhasFeature.end(),
                                linhasClasse.begin(), linhasClasse.end(),
                                inserter(linhasIntersecao, linhasIntersecao.begin()));
                if (!linhasIntersecao.empty()) {
                    double suporte = static_cast<double>(linhasIntersecao.size()) / totalLinhas;
                    relevanciaClasse[classe.first] += suporte;
                    }
                }
            }
            vector<pair<int, double>> suporteClasses(relevanciaClasse.begin(), relevanciaClasse.end());
            sort(suporteClasses.begin(), suporteClasses.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
                return a.second > b.second;
            });

            if (!suporteClasses.empty()) {
                return suporteClasses.front().first;
            } else {
                return -1; // Nenhuma classe encontrada
            }
        }
        else if(interacao==2){
            for(int i=0;i<11;i++){
                for(int j=i+1;j<11;j++){
                set<int> intersecao;
                set_intersection(linhas[i].begin(), linhas[j].end(),linhas[i].begin(), linhas[j].end(),inserter(intersecao, intersecao.begin()));
                cout << "Interseccao: ";
                for (const auto& linha : intersecao) {
                            cout << linha << " ";
                        }
                cout << endl;
                }
            }
        }
        else {
            return -1;
        }
    }
}

void teste(const string& nomeArquivoTeste) {;
    ifstream arquivoTeste(nomeArquivoTeste);
    ofstream arquivoSaida("dataset/output.txt");
    vector<int> classesTeste;
    string linha;
    int totalLinhasArquivo = 0;
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
            int classeOriginal = linhaValores.back();
            classesTeste.push_back(classeOriginal);
            linhaValores.pop_back();

            for (size_t i = 0; i < linhaValores.size(); ++i) {
                linhaTuplas.push_back(make_tuple(i + 1, linhaValores[i]));
            }

            int classeAtribuida = avaliarClasse(tabelaHashTreino, tabelaHashClassesTreino, linhaTuplas, totalLinhas);

            arquivoSaida << "Linha " << (totalLinhasArquivo + 1) << ": Classe Atribuída = " << classeAtribuida << endl;

            if (classeAtribuida == classeOriginal) {
                acertos++;
            } else {
                erros++;
            }

            totalLinhasArquivo++;
        }
    }

    arquivoSaida << "Total de acertos: " << acertos << endl;
    arquivoSaida << "Total de erros: " << erros << endl;

    arquivoTeste.close();
    arquivoSaida.close();
}
