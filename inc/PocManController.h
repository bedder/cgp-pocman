//
// Created by bedder on 16/10/2015.
//

#ifndef CGP_POCMAN_POCMANCONTROLLER_H
#define CGP_POCMAN_POCMANCONTROLLER_H

#include "PocManState.h"

class PocManController {
public:
    PocManController();
    PocManController(PocManLevel level);

    double runGame(struct parameters *params,
                   struct chromosome *chromo,
                   struct dataSet *data) const;
    virtual Action getAction(struct chromosome *chromo) const;

protected:
    const PocManLevel level_;
    const unsigned int nMaxTurns_ = 200;
    const unsigned int nRepeats_  = 10;
};

#endif //CGP_POCMAN_POCMANCONTROLLER_H
