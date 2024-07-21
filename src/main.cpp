#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <set>
#include "treinamento.hpp"

using namespace std;


int main() {
    string nomeArquivo = "dataset/input.txt";
    vector<int> classes; // Vetor para armazenar as classes
    vector<vector<tuple<int,int>>> tuplas = lerArquivo(nomeArquivo, classes);

    // Criar tabelas hash
    unordered_map<tuple<int, int>, set<int>> tabelaHash = criarTabelaHash(tuplas);
    unordered_map<int, set<int>> tabelaHashClasses = criarTabelaHashClasses(classes);

    // Impressão das tuplas
    for (size_t i = 0; i < tuplas.size(); ++i) {
        for (const auto& t : tuplas[i]) {
            cout << "Índice: [" << get<0>(t) << "] Valor: [" << get<1>(t) << "] ";
        }
        cout << "| Classe: " << classes[i] << endl;
    }

    // Impressão da Tabela Hash de Linhas
    cout << "\nTabela Hash:" << endl;
    for (const auto& [chave, linhas] : tabelaHash) {
        cout << "Chave: (Índice: " << get<0>(chave) << ", Valor: " << get<1>(chave) << ") - Aparece nas linhas: ";
        for (const auto& linha : linhas) {
            cout << linha << " ";
        }
        cout << endl;
    }

    // Impressão da Tabela Hash das Classes
    cout << "\nTabela Hash das Classes:" << endl;
    for (const auto& [classe, linhas] : tabelaHashClasses) {
        cout << "Classe: " << classe << " Aparece nas linhas: ";
        for (const auto& linha : linhas) {
            cout << linha << " ";
        }
        cout << endl;
    }

    int colunaParaBuscar;
    int valorParaBuscar;

    cout << "Digite o número da coluna para buscar (iniciando em 1): ";
    cin >> colunaParaBuscar;
    
    cout << "Digite o valor para buscar: ";
    cin >> valorParaBuscar;

    // Verificar se a entrada é válida
    if (colunaParaBuscar < 1) {
        cerr << "O número da coluna deve ser maior ou igual a 1." << endl;
        return 1;
    }

    // Buscar na tabela hash
    set<int> linhas = buscarFeature(tabelaHash, colunaParaBuscar, valorParaBuscar);

    // Impressão dos resultados da busca
    cout << "Busca para (Coluna: " << colunaParaBuscar << ", Valor: " << valorParaBuscar << "):" << endl;
    if (!linhas.empty()) {
        cout << "Aparece nas linhas: ";
        for (const auto& linha : linhas) {
            cout << linha << " ";
        }
        cout << endl;
    } else {
        cout << "Nenhum resultado encontrado." << endl;
    }

    return 0;
}
