#include <iostream>
#include "PocManController.h"
#include "PocManState.h"
#include "cgp.h"

PocManController controller;
double fitness(struct parameters *params,
               struct chromosome *chromo,
               struct dataSet *data) {
	return controller.runGame(params, chromo, data);
}

int main() {
	// Define the CGP constants
	struct parameters *params = NULL;
	struct dataSet *trainingData = NULL;
	struct chromosome *chromo = NULL;
	int numInputs  =  9; // 4*touch, 4*sight, 1*smell
	int numNodes   = 15; 
	int numOutputs =  4; // North, East, South, West
	int nodeArity  =  2; // Standard for node
	int numGens    = 10000;
	int updateFrequency = 500;
	double targetFitness = -10000;

	// Start the evolution!
	params = initialiseParameters(numInputs, numNodes, numOutputs, nodeArity);
	addNodeFunction(params, "add,sub,mul,div,sin");
	setTargetFitness(params, targetFitness);
	setUpdateFrequency(params, updateFrequency);
	printParameters(params);
	setCustomFitnessFunction(params, fitness, "PocMan Level 1");
	chromo = runCGP(params, trainingData, numGens);

	// Print the output
	printChromosome(chromo, 0);

	// Tidy up
	freeDataSet(trainingData);
	freeChromosome(chromo);
	freeParameters(params);

	std::cin.ignore();

	return 0;
}