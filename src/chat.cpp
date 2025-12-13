#include "chat.h"
#include <iostream>
#include <string>

using namespace std;

void Chat::join(string username) { cout << "Joining as: " << username << endl; }

void Chat::render() { cout << "Redenring messages..." << endl; }

void Chat::handleInput(string input) {
  if (input == "/exit" || input == "/quit") {
    Chat::stopRunning();
    return;
  }
}
