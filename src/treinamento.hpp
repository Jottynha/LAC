#ifndef TREINAMENTO_HPP
#define TREINAMENTO_HPP

#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>
#include <set>
using namespace std;

// Função para ler valores inteiros de um arquivo e retornar um vetor de tuplas
vector<vector<tuple<int,int>>> lerArquivo(const string& nomeArquivo, vector<int>& classes);
unordered_map<int, set<int>> criarTabelaHash(const vector<vector<tuple<int, int>>>& tuplas);
unordered_map<int, set<int>> criarTabelaHashClasses(const vector<int>& classes);

#endif