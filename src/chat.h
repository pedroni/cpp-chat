#pragma once

#include <string>
#include <vector>
using namespace std;

class Chat {
private:
  bool _running = true;
  string _username;

  vector<string> _welcomeMessages;
  vector<string> _messages;

public:
  string input;
  char currentCh;

  Chat() {
    _welcomeMessages.push_back("Look who's here—{Name}!");
    _welcomeMessages.push_back("{Name} has entered the chat.");
    _welcomeMessages.push_back("Ah, {Name}. Let's get into it.");
    _welcomeMessages.push_back("Ready when you are, {Name}.");
    _welcomeMessages.push_back("{Name}! What's on your mind?");
    _welcomeMessages.push_back(
        "Another day, another conversation with {Name}.");
    _welcomeMessages.push_back("The one and only {Name}.");
    _welcomeMessages.push_back("{Name} is in the building.");
    _welcomeMessages.push_back("What shall we tackle today, {Name}?");
    _welcomeMessages.push_back("Good to see you, {Name}.");
    _welcomeMessages.push_back("{Name}—let's make this a good one.");
    _welcomeMessages.push_back("Back again, {Name}?");
    _welcomeMessages.push_back("Hey there, {Name}. What's up?");
    _welcomeMessages.push_back("{Name} approaches. Adventure awaits.");
    _welcomeMessages.push_back("Greetings, {Name}. Fire away.");
  }

  void join(string username);

  void handleInput();

  void render() const;
  void renderMessages() const;

  bool isRunning() const { return _running; };

  void stopRunning() { _running = false; }
};
