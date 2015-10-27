#include "Experiment.h"

#include <ctime>
#include <chrono>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

#include "cgp.h"
#include "PocManController.h"

// Globals (eugh!)
PocManController controller;
std::ofstream logFile;
std::string filenameFormat = "%Y%m%d_%H%M%S.txt";

// Simple fitness function: provides a wrapper for the member function of the
// global PocManController "controller".
double fitness(struct parameters *params,
               struct chromosome *chromo,
               struct dataSet *data) {
	return controller.runGame(params, chromo, data);
}

// Simple logging functionality: if the log file isn't already open, it tries to
// open it. If it can't, it throws a tantrum.
void log(const std::string& str) {
  if (!logFile.is_open()) {
    // Try to open the file 
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    char buffer[80];
    strftime(buffer, 80, filenameFormat.c_str(), &tm);
    logFile.open(buffer,
                 std::ofstream::out);
    if (!logFile.is_open()) {
      std::cerr << "Unable to open log file.";
      exit(1);
    }
  }
  // The file is open!
  logFile << str;
}

void generateTrace(struct chromosome *chromo) {

}

// Calculates the mean and standard deviation from a std::vector<double>
std::tuple<double, double, double, double> calculateStats(const std::vector<double>& vec) {
  int n = vec.size();
  double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
  double mean = sum / n;
  double squareOfMean = mean * mean;
  double sumOfSquares = std::inner_product(vec.begin(), vec.end(),
                                           vec.begin(),
                                           0.0);
  double meanOfSquare = sumOfSquares / n;
  double stdev = std::sqrt(meanOfSquare - squareOfMean);
  double min = *std::min_element(vec.begin(), vec.end());
  double max = *std::max_element(vec.begin(), vec.end());
  return std::make_tuple(mean, stdev, min, max);
}

// Runs a full round of experiments
void performExperiments(struct parameters* params,
                        unsigned int nGenerations,
                        unsigned int nRepeats,
                        const std::string& nodeFunctions) {
  setCustomFitnessFunction(params, fitness, "PocMan Level 1");

  struct chromosome* bestChromosome = nullptr;
  double bestFitness = DBL_MAX;
  std::vector<double> fitnesses;
  std::vector<double> durations;
  fitnesses.reserve(nRepeats);
  durations.reserve(nRepeats);

  for (unsigned int i = 0; i < nRepeats; i++) {
    // Run CGP, record the results
    auto start = std::chrono::steady_clock::now();
    struct chromosome* chromo = runCGP(params, nullptr, nGenerations);
    auto end = std::chrono::steady_clock::now();

    double fitness = getChromosomeFitness(chromo);
    fitnesses.push_back(fitness);

    std::chrono::duration<double> diff = end - start;
    double duration = diff.count();
    durations.push_back(duration);

    // Should we keep this chromosome?
    if (fitness < bestFitness) {
      if (bestChromosome != nullptr)
        freeChromosome(bestChromosome);
      bestChromosome = chromo;
      bestFitness = fitness;
    } else {
      freeChromosome(chromo);
    }
  }

  // Stats!
  double meanFitness, stdevFitness, minFitness, maxFitness;
  std::tie(meanFitness, stdevFitness, minFitness, maxFitness) = calculateStats(fitnesses);

  double meanDuration, stdevDuration, minDuration, maxDuration;
  std::tie(meanDuration, stdevDuration, minDuration, maxDuration) = calculateStats(durations);

  // Logging!
  log("### PARAMETERS\n");
  log(parametersToString(params));
  log("\n--------------------\n\n");
  log("### FITNESS VALUES\n");
  for (unsigned int i = 0; i < nRepeats; i++)
    log("(" + std::to_string(i + 1) + "):\t" + std::to_string(fitnesses[i]) + "\n");
  log("\n--------------------\n\n");
  log("### STATISTICS (FITNESS)\n");
  log("mean : " + std::to_string(meanFitness) + "\n");
  log("std  : " + std::to_string(stdevFitness) + "\n");
  log("rng  : " + std::to_string(minFitness) +":" + std::to_string(maxFitness) + "\n");
  log("\n--------------------\n\n");
  log("### DURATION VALUES\n");
  for (unsigned int i = 0; i < nRepeats; i++)
    log("(" + std::to_string(i + 1) + "):\t" + std::to_string(durations[i]) + "\n");
  log("\n--------------------\n\n");
  log("### STATISTICS (DURATIONS)\n");
  log("mean : " + std::to_string(meanDuration) + "\n");
  log("std  : " + std::to_string(stdevDuration) + "\n");
  log("rng  : " + std::to_string(minDuration) + ":" + std::to_string(maxDuration) + "\n");
  log("\n--------------------\n\n");
  log("### BEST CHROMOSOME\n");
  log(chromosomeToString(bestChromosome, 0));
  log("\n--------------------\n\n");
  log("### GAMEPLAY TRACE\n");
  log(controller.generateGameTrace(bestChromosome));
}