#include <iostream>
#include "treinamento.hpp"
#include "teste.hpp"

using namespace std;


int main() {
    selecionarLinhasAleatorias("dataset/poker-hand-testing.data", "dataset/20linhas.txt", 100);
    treinamento("dataset/poker-hand-training.data");
    teste("dataset/20linhas.txt");
    return 0;
}
