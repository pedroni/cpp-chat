#include "chat.h"
#include <chrono>
#include <fmt/printf.h>
#include <iostream>
#include <ncurses.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>

using namespace std;

void readInput(Chat &chat) {
  while (chat.isRunning()) {
    chat.currentCh = getch();

    switch ((int)chat.currentCh) {
    case 10:
      chat.handleInput();
      chat.input = "";
      break;
    default:
      chat.input += chat.currentCh;
      break;
    }
  }
}

void renderChat(const Chat &chat) {
  constexpr int fps = 240;
  constexpr int msPerFrame = 1000 / fps;

  while (chat.isRunning()) {
    chat.render();

    // fakes running at specific timestep
    this_thread::sleep_for(chrono::milliseconds(msPerFrame));
  }
}

int main() {
  std::cout << "we dont have anything do we?" << std::endl;

  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo;

  // make sure hints is empty, when initializing structs it fills with garbage
  // data
  memset(&hints, 0, sizeof hints);

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // getaddrinfo();

  return 0;
  // ACTUAL CHAT CODE
  // ACTUAL CHAT CODE
  // ACTUAL CHAT CODE
  // ACTUAL CHAT CODE

  // clears the screen and presents a virtual screen
  initscr();

  // with the default c++ behavior everytime we type something on the terminal
  // using std::cin we would see the output of the keys that are pressed, this
  // hides whatever key is pressed
  noecho();

  // enables special keys to be used with constant KEY_ENTER for example, this
  // also enables the usage of F1, F2 keys
  // dunno? didnt work with KEY_ENTER had to use int(10)
  keypad(stdscr, true);

  Chat chat;

  thread input{readInput, ref(chat)};
  thread renderer{renderChat, ref(chat)};

  // join in threads works in similar fashion as an await in javascript
  input.join();
  renderer.join();

  // ends the program, this goes back to the original terminal output, and frees
  // the memory allocated by ncurse
  endwin();

  std::cout << "Sad to see you go!" << std::endl;

  return 0;
}
