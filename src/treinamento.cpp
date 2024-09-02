#include "treinamento.hpp"

// Definição das tabelas hash e tuplas como variáveis globais
unordered_map<tuple<int, int>, set<int>> tabelaHashTreino;
unordered_map<int, set<int>> tabelaHashClassesTreino;
vector<vector<tuple<int, int>>> tuplasTreino;
int totalLinhas = 0;

// Definição da função para ler valores inteiros de um arquivo
vector<vector<tuple<int, int>>> lerArquivo(const string &nomeArquivo,
                                           vector<int> &classes) {
  ifstream arquivo(nomeArquivo);
  vector<vector<tuple<int, int>>> tuplas;
  string linha;

  if (!arquivo) {
    cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
    return tuplas;
  }

  while (getline(arquivo, linha)) {
    vector<tuple<int, int>> linhaTuplas;
    stringstream ss(linha);
    string item;
    vector<int> linhaValores;

    while (getline(ss, item, ',')) {
      linhaValores.push_back(stoi(item));
    }

    // O último valor é a classe, então removemos ele da linha de valores
    if (!linhaValores.empty()) {
      classes.push_back(linhaValores.back());
      linhaValores.pop_back();
    }

    // Criar pares de valores na mesma linha
    for (size_t i = 0; i < linhaValores.size(); ++i) {
      for (size_t j = i + 1; j < linhaValores.size(); ++j) {
        linhaTuplas.push_back(make_tuple(linhaValores[i], linhaValores[j]));
      }
    }

    tuplas.push_back(linhaTuplas);
  }

  arquivo.close();
  return tuplas;
}

// Definição da função para criar uma tabela hash a partir do vetor de tuplas
unordered_map<tuple<int, int>, set<int>>
criarTabelaHash(const vector<vector<tuple<int, int>>> &tuplas) {
  unordered_map<tuple<int, int>, set<int>> tabelaHash;

  for (size_t i = 0; i < tuplas.size(); ++i) {
    for (const auto &t : tuplas[i]) {
      tabelaHash[t].insert(i + 1);
    }
  }

  return tabelaHash;
}

// Definição da função para criar uma tabela hash para as classes
unordered_map<int, set<int>>
criarTabelaHashClasses(const vector<int> &classes) {
  unordered_map<int, set<int>> tabelaHashClasses;

  for (size_t i = 0; i < classes.size(); ++i) {
    tabelaHashClasses[classes[i]].insert(i + 1);
  }

  return tabelaHashClasses;
}

set<int>
buscarFeature(const unordered_map<tuple<int, int>, set<int>> &tabelaHash,
              int valor1, int valor2) {
  tuple<int, int> chave = make_tuple(valor1, valor2);
  auto it = tabelaHash.find(chave);

  if (it != tabelaHash.end()) {
    return it->second; // Retorna o conjunto de linhas onde a chave aparece
  } else {
    return {}; // Retorna um conjunto vazio se a chave não for encontrada
  }
}

set<int> buscarClasse(const unordered_map<int, set<int>> &tabelaHashClasses,
                      int classe) {
  auto it = tabelaHashClasses.find(classe);
  if (it != tabelaHashClasses.end()) {
    return it->second; // Retorna o conjunto de linhas onde a classe aparece
  } else {
    return {}; // Retorna um conjunto vazio se a classe não for encontrada
  }
}

void treinamento(string nomeArquivo) {
  vector<int> classes; // Vetor para armazenar as classes
  tuplasTreino = lerArquivo(nomeArquivo, classes);
  tabelaHashTreino = criarTabelaHash(tuplasTreino);
  tabelaHashClassesTreino = criarTabelaHashClasses(classes);
  totalLinhas = tuplasTreino.size();
}
