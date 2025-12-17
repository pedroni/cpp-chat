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
    _welcomeMessages.push_back("Look who's here - %s!");
    _welcomeMessages.push_back("%s has entered the chat.");
    _welcomeMessages.push_back("Ah, %s. Let's get into it.");
    _welcomeMessages.push_back("Ready when you are, %s.");
    _welcomeMessages.push_back("%s! What's on your mind?");
    _welcomeMessages.push_back("Another day, another conversation with %s.");
    _welcomeMessages.push_back("The one and only %s.");
    _welcomeMessages.push_back("%s is in the building.");
    _welcomeMessages.push_back("What shall we tackle today, %s?");
    _welcomeMessages.push_back("Good to see you, %s.");
    _welcomeMessages.push_back("%s—let's make this a good one.");
    _welcomeMessages.push_back("Back again, %s?");
    _welcomeMessages.push_back("Hey there, %s. What's up?");
    _welcomeMessages.push_back("%s approaches. Adventure awaits.");
    _welcomeMessages.push_back("Greetings, %s. Fire away.");
  }

  void join(string username);

  void handleInput();

  void restart();
  void sendMessage(string message);

  void render() const;
  void renderMessages() const;

  bool isRunning() const { return _running; };

  void stopRunning() { _running = false; }
};
