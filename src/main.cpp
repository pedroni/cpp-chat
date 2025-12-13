#include "chat.h"
#include <chrono>
#include <functional>
#include <iostream>
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
  cout << "=========== CHAT ===========" << endl;

  Chat chat;

  thread input{readInput, ref(chat)};
  thread renderer{renderChat, ref(chat)};

  // join in threads works in similar fashion as an await in javascript
  input.join();
  renderer.join();

  cout << "======== CHAT ENDED ========" << endl;
  return 0;
}
