#ifndef TREINAMENTO_HPP
#define TREINAMENTO_HPP

#include <vector>
#include <tuple>
#include <string>
using namespace std;

// Função para ler valores inteiros de um arquivo e retornar um vetor de tuplas
vector<vector<tuple<int>>> lerArquivo(const string& nomeArquivo);

#endif