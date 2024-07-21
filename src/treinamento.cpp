#include "treinamento.hpp"

// Definição das tabelas hash e tuplas como variáveis globais
unordered_map<tuple<int, int>, set<int>> tabelaHashTreino;
unordered_map<int, set<int>> tabelaHashClassesTreino;
vector<vector<tuple<int, int>>> tuplasTreino;

// Definição da função para ler valores inteiros de um arquivo
vector<vector<tuple<int,int>>> lerArquivo(const string& nomeArquivo, vector<int>& classes) {
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

        // Adiciona tuplas contendo o índice e o valor
        for (size_t i = 0; i < linhaValores.size(); ++i) {
            linhaTuplas.push_back(make_tuple(i+1, linhaValores[i]));
        }

        tuplas.push_back(linhaTuplas);
    }

    arquivo.close();
    return tuplas;
}

// Definição da função para criar uma tabela hash a partir do vetor de tuplas
unordered_map<tuple<int, int>, set<int>> criarTabelaHash(const vector<vector<tuple<int, int>>>& tuplas) {
    unordered_map<tuple<int, int>, set<int>> tabelaHash;

    for (size_t i = 0; i < tuplas.size(); ++i) {
        for (const auto& t : tuplas[i]) {
            tabelaHash[t].insert(i + 1);
        }
    }

    return tabelaHash;
}

// Definição da função para criar uma tabela hash para as classes
unordered_map<int, set<int>> criarTabelaHashClasses(const vector<int>& classes) {
    unordered_map<int, set<int>> tabelaHashClasses;

    for (size_t i = 0; i < classes.size(); ++i) {
        tabelaHashClasses[classes[i]].insert(i + 1);
    }

    return tabelaHashClasses;
}

set<int> buscarFeature(const unordered_map<tuple<int, int>, set<int>>& tabelaHash, int coluna, int valor) {
    tuple<int, int> chave = make_tuple(coluna, valor);
    auto it = tabelaHash.find(chave);

    if (it != tabelaHash.end()) {
        return it->second; // Retorna o conjunto de linhas onde a chave aparece
    } else {
        return {}; // Retorna um conjunto vazio se a chave não for encontrada
    }
}

set<int> buscarClasse(const unordered_map<int, set<int>>& tabelaHashClasses, int classe) {
    auto it = tabelaHashClasses.find(classe);
    if (it != tabelaHashClasses.end()) {
        return it->second; // Retorna o conjunto de linhas onde a classe aparece
    } else {
        return {}; // Retorna um conjunto vazio se a classe não for encontrada
    }
}

void treinamento(string nomeArquivo){
    vector<int> classes; // Vetor para armazenar as classes
    tuplasTreino = lerArquivo(nomeArquivo, classes);
    tabelaHashTreino = criarTabelaHash(tuplasTreino);
    tabelaHashClassesTreino = criarTabelaHashClasses(classes);
    
    // Impressão das tuplas
    for (size_t i = 0; i < tuplasTreino.size(); ++i) {
        for (const auto& t : tuplasTreino[i]) {
            cout << "Índice: [" << get<0>(t) << "] Valor: [" << get<1>(t) << "] ";
        }
        cout << "| Classe: " << classes[i] << endl;
    }

    // Impressão da Tabela Hash de Linhas
    cout << "\nTabela Hash:" << endl;
    for (const auto& [chave, linhas] : tabelaHashTreino) {
        cout << "Chave: (Índice: " << get<0>(chave) << ", Valor: " << get<1>(chave) << ") - Aparece nas linhas: ";
        for (const auto& linha : linhas) {
            cout << linha << " ";
        }
        cout << endl;
    }

    // Impressão da Tabela Hash das Classes
    cout << "\nTabela Hash das Classes:" << endl;
    for (const auto& [classe, linhas] : tabelaHashClassesTreino) {
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
    if (colunaParaBuscar < 1) {
        cerr << "O número da coluna deve ser maior ou igual a 1." << endl;
    }
    set<int> linhas = buscarFeature(tabelaHashTreino, colunaParaBuscar, valorParaBuscar); // Buscar na tabela hash
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

    int classeParaBuscar;
    cout << "\nDigite a classe para buscar: ";
    cin >> classeParaBuscar;
    set<int> linhasClasse = buscarClasse(tabelaHashClassesTreino, classeParaBuscar); // Buscar na tabela hash de classes
    cout << "Busca para Classe: " << classeParaBuscar << ":" << endl;
    if (!linhasClasse.empty()) {
        cout << "Aparece nas linhas: ";
        for (const auto& linha : linhasClasse) {
            cout << linha << " ";
        }
        cout << endl;
    } else {
        cout << "Nenhum resultado encontrado." << endl;
    }

}