#include "chat.h"
#include "utils/rand.h"
#include <_stdlib.h>
#include <fmt/printf.h>
#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

void Chat::join(string username) {
  _username = username;

  string welcomeMessage =
      _welcomeMessages.at(Rand::generate(0, _welcomeMessages.size()));

  _messages.push_back(fmt::sprintf(welcomeMessage, username));
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

  if (input == "/restart") {
    Chat::restart();
    return;
  }

  if (Chat::_username == "") {
    Chat::join(input);
    return;
  }

  Chat::sendMessage(input);
}

void Chat::renderMessages() const {
  for (auto message : _messages) {
    printw("%s\n", message.c_str());
  }
}

void Chat::restart() {
  _username = "";
  _messages = vector<string>();
}

void Chat::sendMessage(string message) {
  _messages.push_back(fmt::sprintf("%s: %s", _username, message));
}
