#include <iostream>
#include "treinamento.hpp"
#include "teste.hpp"

using namespace std;


int main() {
    treinamento("dataset/treinamento.txt");
    teste("dataset/teste.txt");
    return 0;
}
