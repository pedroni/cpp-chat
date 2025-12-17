#pragma once

#include <climits>
#include <random>

using namespace std;

class Rand {
private:
public:
  static int generate() { return generate(0, INT_MAX); }

  static int generate(int low, int high) {
    static std::random_device randomDevice;
    static std::mt19937 gen(randomDevice());
    std::uniform_int_distribution<> generator(low, high);

    return generator(gen);
  }
};
