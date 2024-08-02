#include <iostream>
#include "treinamento.hpp"
#include "teste.hpp"

using namespace std;


int main() {
    treinamento("dataset/poker-hand-training.data");
    teste("dataset/poker-hand-testing.data");
    return 0;
}
