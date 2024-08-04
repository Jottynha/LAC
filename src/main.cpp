#include <iostream>
#include "treinamento.hpp"
#include "teste.hpp"

using namespace std;


int main() {
    treinamento("dataset/iris.data");
    teste("dataset/teste.txt");
    return 0;
}
