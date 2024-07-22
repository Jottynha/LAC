#ifndef TESTE_HPP
#define TESTE_HPP
#include "treinamento.hpp"
#include <algorithm>
#include <numeric> 

int avaliarClasse(const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                  const unordered_map<int, set<int>>& tabelaHashClasses,
                  const vector<tuple<int, int>>& featuresLinha, int totalLinhas);
void teste(const string& nomeArquivoTeste);

#endif