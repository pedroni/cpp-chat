#include "chat.h"
#include <chrono>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <thread>

using namespace std;

void readInput(Chat &chat) {
  char rawInput;
  string input;
  while (chat.isRunning() && cin.get(rawInput)) {
    input += rawInput;

    cout << "The raw input" << endl;
    chat.handleInput(input);
    input = "";
  }
}

void renderChat(const Chat &chat) {
  while (chat.isRunning()) {
    chat.render();

    // runs the chat at roughly 5fps, we don't need more than that, 200ms per
    // frame, 5 frames in a second
    this_thread::sleep_for(chrono::milliseconds(200));
  }
}

int main() {
  // clears the screen and presents a virtual screen
  initscr();

  // enables capturing input by input without having to wait the user to press
  // enter, pressing enter to get input is the default from std::cin
  // cbreak();
  // raw();

  // with the default c++ behavior everytime we type something on the terminal
  // using std::cin we would see the output of the keys that are pressed, this
  // hides whatever key is pressed
  noecho();

  // works just like printf, but writing to curse screen

  // mvprintw moves characters positioning on the screen
  mvprintw(0, 5, "=========== CHAT ===========\n");

  // actually prints the stuff on the curse screen, you should do as many
  // things as possible and then call refresh to render it
  refresh();

  // get input
  char str[90];
  // getstr reads a line
  // where as getch reads a single character based of cbreak()
  getstr(str);
  printw("You typed %s", str);

  // https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/scanw.html
  refresh();
  getch();
  printw("========= END CHAT =========\n");

  getch();
  // ends the program, this goes back to the original terminal output, and frees
  // the memory allocated by ncurse
  endwin();

  // Chat chat;

  // thread input{readInput, ref(chat)};
  // thread renderer{renderChat, ref(chat)};

  // // join in threads works in similar fashion as an await in javascript
  // input.join();
  // renderer.join();
  return 0;
}
