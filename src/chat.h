#pragma once

#include <string>
#include <vector>
using namespace std;

class Chat {
private:
  bool _running = true;
  string _username;

  vector<string> messages;

public:
  Chat() {
    messages.push_back("Look who's here—{Name}!");
    messages.push_back("{Name} has entered the chat.");
    messages.push_back("Ah, {Name}. Let's get into it.");
    messages.push_back("Ready when you are, {Name}.");
    messages.push_back("{Name}! What's on your mind?");
    messages.push_back("Another day, another conversation with {Name}.");
    messages.push_back("The one and only {Name}.");
    messages.push_back("{Name} is in the building.");
    messages.push_back("What shall we tackle today, {Name}?");
    messages.push_back("Good to see you, {Name}.");
    messages.push_back("{Name}—let's make this a good one.");
    messages.push_back("Back again, {Name}?");
    messages.push_back("Hey there, {Name}. What's up?");
    messages.push_back("{Name} approaches. Adventure awaits.");
    messages.push_back("Greetings, {Name}. Fire away.");
  }

  void join(string username);

  void handleInput(string input);

  void render() const;

  bool isRunning() const { return _running; };

  void stopRunning() { _running = false; }
};
