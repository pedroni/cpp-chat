#include "chat.h"
#include <iostream>
#include <ncurses.h>
#include <string>

using namespace std;

void Chat::join(string username) { _username = username; }

void Chat::render() const {
  // printw moves characters positioning on the screen
  clear();
  printw("=========== CHAT =========== (%d) \n", currentCh);

  if (input != "") {
    printw("%s", input.c_str());
  }

  // if (Chat::_username == "") {
  //   std::cout << "How can we call you?" << std::endl;
  // } else {
  //   cout << "Welcome, " << _username << endl;
  // }

  // refresh actually prints the stuff on the curse screen, you should do as
  // many things as possible and then call refresh to render it
  refresh();
}

void Chat::handleInput() {
  if (input == "/exit" || input == "/quit") {
    Chat::stopRunning();
    return;
  }

  if (Chat::_username == "") {

    Chat::join(input);

    return;
  }
}
