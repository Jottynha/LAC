#ifndef TREINAMENTO_HPP
#define TREINAMENTO_HPP

#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>
#include <set>
using namespace std;

namespace std {
    template <typename... Types>
    struct hash<std::tuple<Types...>> {
        size_t operator()(const std::tuple<Types...>& t) const {
            return hash_tuple(t, std::index_sequence_for<Types...>{});
        }
    private:
        template <std::size_t... I>
        size_t hash_tuple(const std::tuple<Types...>& t, std::index_sequence<I...>) const {
            size_t seed = 0;
            (..., (seed ^= hash<std::decay_t<decltype(std::get<I>(t))>>{}(std::get<I>(t)) + 0x9e3779b9 + (seed << 6) + (seed >> 2)));
            return seed;
        }
    };
}

vector<vector<tuple<int,int>>> lerArquivo(const string& nomeArquivo, vector<int>& classes);
unordered_map<tuple<int, int>, set<int>> criarTabelaHash(const vector<vector<tuple<int, int>>>& tuplas);
unordered_map<int, set<int>> criarTabelaHashClasses(const vector<int>& classes);
set<int> buscarFeature(const unordered_map<tuple<int, int>, set<int>>& tabelaHash, int coluna, int valor);

#endif