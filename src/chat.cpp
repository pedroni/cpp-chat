#include "chat.h"
#include <iostream>
#include <string>

using namespace std;

void Chat::join(string username) { _username = username; }

void Chat::render() const {
  // the fuck. i would never be able to come up with this
  std::cout << "\033[2J\033[H"; // clears the screen before redrawing
  // everytime

  if (Chat::_username == "") {
    std::cout << "How can we call you?" << std::endl;
  } else {
    cout << "Welcome, " << _username << endl;
  }
}

void Chat::handleInput(string input) {
  if (input == "/exit" || input == "/quit") {
    Chat::stopRunning();
    return;
  }

  if (Chat::_username == "") {

    Chat::join(input);

    return;
  }
}
