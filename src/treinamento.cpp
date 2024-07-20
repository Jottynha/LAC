#include "treinamento.hpp"
#include <fstream>
#include <sstream>
#include <iostream>


// Definição da função para ler valores inteiros de um arquivo
vector<vector<tuple<int>>> lerArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    vector<vector<tuple<int>>> tuplas;
    string linha;

    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return tuplas;
    }

    while (getline(arquivo, linha)) {
        vector<tuple<int>> linhaTuplas;
        stringstream ss(linha);
        string item;
        
        while (getline(ss, item, ',')) {
            linhaTuplas.push_back(make_tuple(stoi(item)));
        }

        tuplas.push_back(linhaTuplas);
    }

    arquivo.close();
    return tuplas;
}
