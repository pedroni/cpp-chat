#include "rand.h"
#include <climits>
#include <random>

using namespace std;

int Rand::generate() { return generate(0, INT_MAX); }
int Rand::generate(int low, int high) {

  static std::random_device randomDevice;
  static std::mt19937 gen(randomDevice());
  std::uniform_int_distribution<> generator(low, high);

  return generator(gen);
}
