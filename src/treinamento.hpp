#ifndef TREINAMENTO_HPP
#define TREINAMENTO_HPP

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
using namespace std;

namespace std { // Estruturando a Hash para aceitar Tuplas
template <typename... Types> struct hash<std::tuple<Types...>> {
  size_t operator()(const std::tuple<Types...> &t) const {
    return hash_tuple(t, std::index_sequence_for<Types...>{});
  }

private:
  template <std::size_t... I>
  size_t hash_tuple(const std::tuple<Types...> &t,
                    std::index_sequence<I...>) const {
    size_t seed = 0;
    (...,
     (seed ^= hash<std::decay_t<decltype(std::get<I>(t))>>{}(std::get<I>(t)) +
              0x9e3779b9 + (seed << 6) + (seed >> 2)));
    return seed;
  }
};
} // namespace std

extern unordered_map<tuple<int, int>, set<int>> tabelaHashTreino;
extern unordered_map<int, set<int>> tabelaHashClassesTreino;
extern vector<vector<tuple<int, int>>> tuplasTreino;
extern int totalLinhas;

vector<vector<tuple<int, int>>> lerArquivo(const string &nomeArquivo,
                                           vector<int> &classes);
unordered_map<tuple<int, int>, set<int>>
criarTabelaHash(const vector<vector<tuple<int, int>>> &tuplas);
unordered_map<int, set<int>> criarTabelaHashClasses(const vector<int> &classes);
set<int>
buscarFeature(const unordered_map<tuple<int, int>, set<int>> &tabelaHash,
              int coluna, int valor);
set<int> buscarClasse(const unordered_map<int, set<int>> &tabelaHashClasses,
                      int classe);
void treinamento(string nomeArquivo);

#endif