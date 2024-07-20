#include <iostream>
#include "treinamento.hpp"

using namespace std;

int main() {
    string nomeArquivo = "input/input.txt";
    vector<vector<tuple<int>>> tuplas = lerArquivo(nomeArquivo);
    // Impressão da tupla:
    for (const auto& linhaTuplas : tuplas) {
        for (const auto& t : linhaTuplas) {
            cout << "Valor: " << get<0>(t) << " ";
        }
        cout << endl;
    }
    return 0;
}
