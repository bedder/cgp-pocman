#ifndef CGP_POCMAN_EXPERIMENT_H
#define CGP_POCMAN_EXPERIMENT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include "cgp.h"
#include "PocManController.h"

// Constants
static const unsigned int N_INPUTS  = 9;  // touch(4), sight(4), smell(1)
static const unsigned int N_OUTPUTS = 4; // N, E, S, W
static const unsigned int FNC_ARITY = 2;
static const unsigned int UPDATE_FREQ = 0; // Don't print updates to the screen
static const double TARGET_FITNESS = 0.0;   // We want no regret

double fitness(struct parameters *params,
               struct chromosome *chromo,
               struct dataSet *data);

void log(const std::string& str);

std::tuple<double, double, double, double> calculateStats(const std::vector<double>& vec);

void performExperiments(struct parameters* params,
                        unsigned int nGenerations,
                        unsigned int nRepeats,
                        const std::string& nodeFunctions);

#endif // CGP_POCMAN_EXPERIMENT_H