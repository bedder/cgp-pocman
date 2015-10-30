//
// Created by bedder on 16/10/2015.
//

#include <algorithm>
#include <iostream>

#include <cgp.h>
#include <float.h>
#include "PocManController.h"

PocManController::PocManController()
        : PocManController(PocManLevel::Default) {
}

PocManController::PocManController(PocManLevel level)
        : level_(level) {
}

double PocManController::runGame(struct parameters *params,
                                 struct chromosome *chromo,
                                 struct dataSet *data) const {
    double cumulativeReward = 0;
    for (unsigned int repeat=0 ; repeat<nRepeats_ ; repeat++) {
        PocManState game = PocManState(level_);
        for (unsigned int turn = 0 ; turn<nMaxTurns_ ; turn++) {
            auto observation = game.getSenses();
            executeChromosome(chromo, &observation[0]);
            // Calculate action from output chromosome
            Action action = getAction(chromo);
            // Apply, and record the reward
            cumulativeReward += game.performAction(action);
            // Are we at a terminal game state?
            if (game.isTerminal()) {
                cumulativeReward += CLR_REWARD;
                break;
            }
        }
    }
    double maxReward = std::min(nMaxTurns_, PocManState(level_).nPellets()) * PLT_REWARD
                       + (nMaxTurns_ > PocManState(level_).nPellets()) * CLR_REWARD;
    double averageReward = cumulativeReward / nRepeats_;

    // Return the regret of the current strategy
    return maxReward - averageReward;
}

std::string PocManController::generateGameTrace(struct chromosome* chromo) const {
    std::stringstream ss;
    PocManState game = PocManState(level_);
    for (unsigned int turn = 0 ; turn<nMaxTurns_ ; turn++) {
        auto observation = game.getSenses();
        executeChromosome(chromo, &observation[0]);
        // Calculate action from output chromosome
        Action action = getAction(chromo);
        // Record the action
        if (action == North)
          ss << "N";
        else if (action == East)
          ss << "E";
        else if (action == South)
          ss << "S";
        else
          ss << "W";
        // Apply the action, and ignore the reward
        game.performAction(action);
        // Are we at a terminal game state?
        if (game.isTerminal()) {
            break;
        }
    }
    return ss.str();
}

Action PocManController::getAction(struct chromosome *chromo) const {
    // Greedy action selection
    Action bestAction;
    double bestValue = -DBL_MAX;
    for (unsigned int i=0; i<4 ; i++) {
        double currentValue = getChromosomeOutput(chromo, i);
        if (currentValue > bestValue) {
            bestValue = currentValue;
            bestAction = (Action)i;
        }
    }
    return bestAction;
}
