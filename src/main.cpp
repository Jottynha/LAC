#include "teste.hpp"
#include "treinamento.hpp"
#include <iostream>

using namespace std;

int main() {

  treinamento("dataset/poker-hand-training.data");
  teste("dataset/poker-hand-testing.data");
  return 0;
}
