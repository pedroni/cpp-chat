#pragma once

#include <string>
using namespace std;

class Chat {
private:
  bool _running = true;
  string _username;

public:
  void join(string username);

  void handleInput(string input);

  void render();

  bool isRunning() const { return _running; };

  void stopRunning() { _running = false; }
};
