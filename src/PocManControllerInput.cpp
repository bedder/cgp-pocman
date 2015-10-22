#include "PocManControllerInput.h"

#include <iostream>
#include <Windows.h>

PocManControllerInput::PocManControllerInput()
        : PocManController() {
}

PocManControllerInput::PocManControllerInput(PocManLevel level)
        : PocManController(level) {
}

void PocManControllerInput::playGame() {
  PocManState game = PocManState(level_);

  // Set up output
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  
  // Print and loop
  unsigned int turn = 0;
  int reward = 0;
  printState(console, game.toString());
  while (turn < nMaxTurns_
         && !game.isTerminal()) {
    Action a = getAction();
    reward += game.performAction(a);
    turn++;
    printState(console, game.toString());
  }

  std::cout << "\n\nReward: " << reward << std::endl;
  while (1)
    std::cin.ignore();
}

void PocManControllerInput::printState(HANDLE& console,
                                       const std::string& state) const {
  SetConsoleCursorPosition(console, {0, 0});
  std::cout << state;
}

Action PocManControllerInput::getAction() const {
  char c;
  while (1) {
    std::cout << "Input a valid action (N,E,S,W):  \b";
    c = getchar();
    c = (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
    if (c == 'n')
      return North;
    if (c == 'e')
      return East;
    if (c == 's')
      return South;
    if (c == 'w')
      return West;
  }
}