#ifndef CGP_POCMAN_POCMANCONTROLLERINPUT_H
#define CGP_POCMAN_POCMANCONTROLLERINPUT_H

#include "PocManController.h"

#include <Windows.h>

class PocManControllerInput : public PocManController {
public:
  PocManControllerInput();
  PocManControllerInput(PocManLevel level);

  void playGame();
  void printState(HANDLE& console, const std::string& state) const;
  Action getAction() const;
};

#endif //CGP_POCMAN_POCMANCONTROLLERINPUT_H