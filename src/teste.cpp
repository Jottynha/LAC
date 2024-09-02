#include "teste.hpp"
#include "treinamento.hpp"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <mutex>
#include <numeric>
#include <set>
#include <sstream>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

const double THRESHOLD_SIMILARIDADE = 0.70;
mutex mutexRelevancia;

// Função para calcular similaridade
double calcularSimilaridade(const vector<int> &linha1,
                            const vector<int> &linha2) {
  int intersecao = 0;
  int uniao = linha1.size();

  for (size_t i = 0; i < linha1.size(); ++i) {
    if (linha1[i] == linha2[i]) {
      intersecao++;
    }
  }

  return static_cast<double>(intersecao) / uniao;
}

// Função para gerar combinações
vector<vector<int>> combinacoes(const vector<int> &indices, int n) {
  vector<vector<int>> result;
  vector<bool> v(indices.size());
  fill(v.begin(), v.begin() + n, true);
  do {
    vector<int> comb;
    for (size_t i = 0; i < indices.size(); ++i) {
      if (v[i]) {
        comb.push_back(indices[i]);
      }
    }
    result.push_back(comb);
  } while (prev_permutation(v.begin(), v.end()));
  return result;
}

void calcularSuportePorCombinacao(
    const vector<set<int>> &linhas, const vector<int> &comb,
    const unordered_map<int, set<int>> &tabelaHashClasses,
    unordered_map<int, double> &relevanciaClasse, int totalLinhas) {
  set<int> intersecao = linhas[comb[0]];
  for (size_t idx = 1; idx < comb.size(); ++idx) {
    set<int> tempIntersecao;
    set_intersection(intersecao.begin(), intersecao.end(),
                     linhas[comb[idx]].begin(), linhas[comb[idx]].end(),
                     inserter(tempIntersecao, tempIntersecao.begin()));
    intersecao = tempIntersecao;
  }

  for (const auto &classe : tabelaHashClasses) {
    set<int> linhasClasse = classe.second;
    set<int> linhasIntersecao;
    set_intersection(intersecao.begin(), intersecao.end(), linhasClasse.begin(),
                     linhasClasse.end(),
                     inserter(linhasIntersecao, linhasIntersecao.begin()));
    if (!linhasIntersecao.empty()) {
      double suporte =
          static_cast<double>(linhasIntersecao.size()) / totalLinhas;
      std::lock_guard<std::mutex> lock(mutexRelevancia);
      relevanciaClasse[classe.first] += suporte;
    }
  }
}

int avaliarClasseCombinatoria(
    const unordered_map<tuple<int, int>, set<int>> &tabelaHash,
    const unordered_map<int, set<int>> &tabelaHashClasses,
    const vector<tuple<int, int>> &featuresLinha, int totalLinhas) {
  unordered_map<int, double> relevanciaClasse;
  vector<set<int>> linhas;

  for (const auto &tupla : featuresLinha) {
    int coluna = get<0>(tupla);
    int valor1 = get<1>(tupla);
    set<int> linhasFeature = buscarFeature(tabelaHash, coluna, valor1);
    if (!linhasFeature.empty()) {
      linhas.push_back(linhasFeature);
    }
  }

  int numLinhas = linhas.size();
  vector<int> indices(numLinhas);
  iota(indices.begin(), indices.end(), 0);

  vector<thread> threads;

  for (int interacao = 1; interacao <= 2; interacao++) {
    auto combs = combinacoes(indices, interacao);
    for (const auto &comb : combs) {
      threads.push_back(thread(calcularSuportePorCombinacao, ref(linhas), comb,
                               ref(tabelaHashClasses), ref(relevanciaClasse),
                               totalLinhas));
    }
  }

  for (auto &t : threads) {
    t.join();
  }

  vector<pair<int, double>> suporteClasses(relevanciaClasse.begin(),
                                           relevanciaClasse.end());

  sort(suporteClasses.begin(), suporteClasses.end(),
       [](const pair<int, double> &a, const pair<int, double> &b) {
         return a.second > b.second;
       });

  if (!suporteClasses.empty()) {
    return suporteClasses.front().first;
  } else {
    return -1;
  }
}

// Função para avaliar a classe de uma linha
int avaliarClasse(const vector<int> &linha,
                  const unordered_map<tuple<int, int>, set<int>> &tabelaHash,
                  const unordered_map<int, set<int>> &tabelaHashClasses,
                  int totalLinhas) {

  vector<tuple<int, int>> featuresLinha;
  for (size_t i = 0; i < linha.size(); ++i) {
    featuresLinha.push_back(make_tuple(i + 1, linha[i]));
  }

  return avaliarClasseCombinatoria(tabelaHash, tabelaHashClasses, featuresLinha,
                                   totalLinhas);
}

// Função para testar o algoritmo com um arquivo de teste
void teste(const string &nomeArquivoTeste) {
  int totalLinhas = 1000;

  ifstream arquivoTeste(nomeArquivoTeste);
  if (!arquivoTeste) {
    cerr << "Erro ao abrir o arquivo de teste." << endl;
    return;
  }

  int totalLinhasArquivo = 0;
  int acertos = 0;
  int erros = 0;
  int a = 0;

  auto inicio = chrono::high_resolution_clock::now();

  string linha;
  while (a < 1000 && getline(arquivoTeste, linha)) {
    a++;
    stringstream ss(linha);
    string item;
    vector<int> linhaValores;
    linhaValores.reserve(1);

    while (getline(ss, item, ',')) {
      linhaValores.push_back(stoi(item));
    }

    int classeReal = linhaValores.back();
    linhaValores.pop_back();

    int classePredita = avaliarClasse(linhaValores, tabelaHashTreino,
                                      tabelaHashClassesTreino, totalLinhas);

    if (classePredita == classeReal) {
      acertos++;
    } else {
      erros++;
    }

    totalLinhasArquivo++;
  }

  auto fim = chrono::high_resolution_clock::now();
  chrono::duration<double> duracao = fim - inicio;

  cout << "Total de linhas no arquivo de teste: " << totalLinhasArquivo << endl;
  cout << "Acertos: " << acertos << endl;
  cout << "Erros: " << erros << endl;
  cout << "Acurácia: " << fixed << setprecision(2)
       << static_cast<double>(acertos) / totalLinhasArquivo * 100 << "%"
       << endl;
  cout << "Tempo total de execução: " << duracao.count() << " segundos."
       << endl;
}
