#include <iostream>
#include <regex>
#include <string>

#include "Experiment.h"

int main(int argc, char **argv) {
  unsigned int nNodes = 15;
  unsigned int nGenerations = 5000;
  unsigned int nRepeats = 20;
  double mutationRate = 0.05;
  unsigned int mu = 1;
  unsigned int lambda = 1;
  char strategy = '+';
  std::string nodeFunctions = "and,or,xor,not";

  int i = 1;
  while (i < argc) {
    if (!strcmp(argv[i], "-nNodes") && i < argc - 1) {
      nNodes = strtoul(argv[++i], NULL, 0);
    } else if (!strcmp(argv[i], "-nGenerations") && i < argc - 1) {
      nGenerations = strtoul(argv[++i], NULL, 0);
    } else if (!strcmp(argv[i], "-nRepeats") && i < argc - 1) {
      nRepeats = strtoul(argv[++i], NULL, 0);
    } else if (!strcmp(argv[i], "-nodeFunctions") && i < argc - 1) {
      nodeFunctions = std::string(argv[++i]);
    } else if (!strcmp(argv[i], "-mutationRate") && i < argc - 1) {
      mutationRate = strtod(argv[++i], NULL);
      mutationRate = (mutationRate > 1 ? 1 : mutationRate);
      mutationRate = (mutationRate < 0 ? 0 : mutationRate);
    } else if (!strcmp(argv[i], "-strategy") && i < argc - 1) {
      std::regex regex("(\\d+)([,+])(\\d+)");
      std::cmatch match;
      if (std::regex_match(argv[++i], match, regex)) {
        mu = atoi(match[1].str().c_str());
        strategy = match[2].str().c_str()[0];
        lambda = atoi(match[3].str().c_str());
      } else {
        std::cerr << "Unable to parse the strategy \"" << argv[i] << "\". Ignoring.\n";
      }
    } else {
      std::cerr << "Unknown argument \"" << argv[i] << "\" provided\n";
      exit(1);
    }
    i++;
  }

  struct parameters *params = initialiseParameters(N_INPUTS,
                                                   nNodes,
                                                   N_OUTPUTS,
                                                   FNC_ARITY);
  addNodeFunction(params, nodeFunctions.c_str());
  setTargetFitness(params, TARGET_FITNESS);
  setUpdateFrequency(params, UPDATE_FREQ);
  setMutationRate(params, mutationRate);
  setMu(params, mu);
  setLambda(params, lambda);
  setEvolutionaryStrategy(params, strategy);

  performExperiments(params, nGenerations, nRepeats, nodeFunctions);

	return 0;
}
