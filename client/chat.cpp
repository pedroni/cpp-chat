#include "chat.h"
#include "rand.h"
#include "time.h"
#include "time_util.h"
#include <_stdlib.h>
#include <cstring>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <ncurses.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace std::chrono;

std::string inet_ntop2(void *addr) {
  char presentable[INET6_ADDRSTRLEN];

  struct sockaddr_storage *sas = (struct sockaddr_storage *)addr;
  struct sockaddr_in *sa4;
  struct sockaddr_in6 *sa6;
  void *src;

  switch (sas->ss_family) {
  case AF_INET:
    sa4 = (struct sockaddr_in *)addr;
    src = &(sa4->sin_addr);
    break;
  case AF_INET6:
    sa6 = (struct sockaddr_in6 *)addr;
    src = &(sa6->sin6_addr);
    break;
  default:
    return "";
  }

  inet_ntop(sas->ss_family, src, presentable, INET6_ADDRSTRLEN);

  return presentable;
}

bool Chat::_connect() {
  int tempSockFd;
  _connectionAttemptedAt = TimeUtil::now();

  struct addrinfo hints, *servinfo, *p;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int rv = getaddrinfo(HOST, PORT, &hints, &servinfo);
  if (rv != 0) {
    _messages.push_back(fmt::sprintf("getaddrinfo: %s\n", gai_strerror(rv)));
    return false;
  }

  int yes = 1;
  p = servinfo;

  // this loop will look through all of the items in the servinfo linked list
  // until it can find an addrinfo that it can BIND to, once it founds it
  // breaks out of the loop
  for (p = servinfo; p != NULL; p = p->ai_next) {
    tempSockFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (tempSockFd == -1) {
      _messages.push_back(
          fmt::sprintf("failed to socket: %s\n", strerror(errno)));
      continue;
    }

    if (::connect(tempSockFd, p->ai_addr, p->ai_addrlen) == -1) {
      continue;
    }
    break;
  }

  // if there's noting on our pointer from the linked list we have to break
  // out of the loop
  if (p == NULL) {
    _messages.push_back(
        fmt::sprintf("Failed to connect to hostname: %s\n", strerror(errno)));

    freeaddrinfo(servinfo);
    return false;
  }

  _messages.push_back(
      fmt::sprintf("Connection established with %s", inet_ntop2(p->ai_addr)));

  // assign the sock fd once connection is properly stabilished
  _sockfd = tempSockFd;

  freeaddrinfo(servinfo);
  return true;
}

void Chat::join(string username) {
  _username = username;

  if (_connect()) {
    string welcomeMessage =
        _welcomeMessages.at(Rand::generate(0, _welcomeMessages.size()));

    _messages.push_back(fmt::sprintf(welcomeMessage, username));
  }

  // we only attempt to reconnect after joining
  _shouldReconnect = true;
}

void Chat::render() const {
  // clears the screen on each render
  clear();

  printw("=========== CHAT =========== (%d) \n", currentCh);

  if (Chat::_username == "") {
    printw("How can we call you?\n");
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

void Chat::_disconnect() {
  close(_sockfd);
  _sockfd = -1;
}

void Chat::restart() {
  _username = "";
  _messages = vector<string>();

  _disconnect();
}

void Chat::sendMessage(string message) {
  string newMessage = fmt::sprintf("%s: %s", _username, message);
  _messages.push_back(newMessage);

  if (send(_sockfd, newMessage.c_str(), newMessage.size(), 0) == -1) {
    _reconnect();
    _messages.push_back(fmt::sprintf(
        "failed to send message %s... reconnecting...", strerror(errno)));
  }
}

// reconnect every 5 seconds
void Chat::_reconnect() {
  if (_username == "") {
    // only attempt to reconnect with a user
    return;
  }
  long long now = TimeUtil::now();

  if (_connectionAttemptedAt == -1 || (now - _connectionAttemptedAt >= 5000)) {
    _disconnect();

    _messages.push_back(("Trying to reconnect..."));
    _connect();
  }
}

void Chat::subscribe() {
  if (!connected()) {
    _reconnect();
    return;
  }

  char buf[256];
  int numBytes = recv(_sockfd, buf, 256, 0);
  if (numBytes == -1) {
    _messages.push_back(
        fmt::sprintf("failed to recv message %s", strerror(errno)));
    _reconnect();
    return;
  }

  if (numBytes > 0) {
    buf[numBytes] = '\0';
    std::string newMessage(buf);
    _messages.push_back(newMessage);
  }
}
