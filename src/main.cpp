#include "chat.h"
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

void readInput(Chat &chat) {
  string input;
  while (chat.isRunning() && getline(cin, input)) {
    chat.handleInput(input);
  }
}

void renderChat(const Chat &chat) {
  while (chat.isRunning()) {
    cout << "rendering the chat..." << endl;

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

  input.join();
  renderer.detach();

  cout << "======== CHAT ENDED ========" << endl;
  return 0;
}
