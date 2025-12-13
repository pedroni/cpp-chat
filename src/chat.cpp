#include "chat.h"
#include <_stdlib.h>
#include <iostream>
#include <ncurses.h>
#include <stdexcept>
#include <string>

using namespace std;

void Chat::join(string username) {
  _username = username;

  string welcomeMessage = _welcomeMessages.at(rand() % _welcomeMessages.size());

  _messages.push_back(welcomeMessage);
}

void Chat::render() const {
  // clears the screen on each render
  clear();

  printw("=========== CHAT =========== (%d) \n", currentCh);

  if (Chat::_username == "") {
    printw("How can we call you?");
  } else {
    renderMessages();
  }

  printw("\n");
  if (input != "") {
    printw("%s", input.c_str());
  }

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

void Chat::renderMessages() const {
  for (auto message : _messages) {
    printw("%s\n", message.c_str());
  }
}
