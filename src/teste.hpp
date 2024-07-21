#ifndef TESTE_HPP
#define TESTE_HPP
#include "treinamento.hpp"
#include <algorithm>
#include <numeric> 

set<int> intersecao(const set<int>& set1, const set<int>& set2);
set<int> buscarFeature(const unordered_map<tuple<int, int>, set<int>>& tabelaHash, int coluna, int valor);
int avaliarCombinacoes(const unordered_map<tuple<int, int>, set<int>>& tabelaHash,
                        const unordered_map<int, set<int>>& tabelaHashClasses,
                        const vector<tuple<int, int>>& featuresLinha);
void teste(const string& nomeArquivoTeste);

#endif