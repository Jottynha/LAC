#include <iostream>
#include "treinamento.hpp"

using namespace std;

int main() {
    string nomeArquivo = "input/input.txt";
    vector<int> classes; // Vetor para armazenar as classes
    vector<vector<tuple<int,int>>> tuplas = lerArquivo(nomeArquivo,classes);
    unordered_map<int, set<int>> tabelaHash = criarTabelaHash(tuplas);
    unordered_map<int, set<int>> tabelaHashClasses = criarTabelaHashClasses(classes);
    // Impressão da tupla:
    for (size_t i = 0; i < tuplas.size(); ++i) {
        for (const auto& t : tuplas[i]) {
            cout << "Índice:[" << get<0>(t) << "] Valor:[" << get<1>(t) << "] ";
        }
        cout << "| Classe: " << classes[i] << endl;
    }
    // Impressão da Tabela Hash de Linhas
    cout << "\nTabela Hash:" << endl;
    for (const auto& [valor, linhas] : tabelaHash) {
        cout << "Valor: " << valor << " - Aparece nas linhas: ";
        for (const auto& linha : linhas) {
            cout << linha << " ";
        }
        cout << endl;
    }
    // Impressão da Tabela Hash de Classes
    cout << "\nTabela Hash das Classes:" << endl;
    for (const auto& [classe, linhas] : tabelaHashClasses) {
        cout << "Classe: " << classe << " Aparece nas linhas: ";
        for (const auto& linha : linhas) {
            cout << linha << " ";  // Já ajustado para 1 baseado na função
        }
        cout << endl;
    }

    return 0;
}
