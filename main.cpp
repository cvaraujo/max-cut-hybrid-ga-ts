#include <sstream>
#include <chrono>
#include <fstream>
#include "Graph.h"
#include "../include/Individual.h"
#include "../include/Population.h"
#include "../include/Algorithm.h"

void checkSolution(Graph *graph, Individual *individual) {
    int i, j, objective = 0;
    for (i = 0; i < graph->getN(); i++) {
        for (auto e : graph->graph[i]) {
            j = e.first;
            if (individual->getGene(i) != individual->getGene(j)) {
                objective += e.second;
            }
        }
    }
    cout << "Objective made from solution: " << objective / 2 << endl;
}

double hybridGeneticAlgorithm(Graph *graph, int timeLimit, int populationSize, int newIndividuals,
                              const string solutionPath) {
    ofstream solution;
    solution.open(solutionPath);

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();

    auto population = new Population(populationSize);
    population->initPopDefault(graph);

    auto *fittest = population->getFittest();

    int generations = 0;

    cout << "Initial population with size " << populationSize << endl;
    cout << "Generating " << newIndividuals << " new individuals per generation" << endl;
    cout << "Running by " << timeLimit << " seconds" << endl;
    cout << "The fittest individual at 1th generation have " << fittest->getFitness() << " of fitness" << endl;

    solution << populationSize << " " << newIndividuals << " " << timeLimit << endl;
    solution << fittest->getFitness() << endl;

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < timeLimit) {
        Algorithm::defaultEvolvePopulation(population, newIndividuals);

        fittest = population->getFittest();
        generations++;
        end = chrono::steady_clock::now();
    }

    cout << "The number of generations was " << generations << endl;
    solution << generations << endl;
    solution << fittest->getFitness() << endl;
    return fittest->getFitness();
}

int main(int argc, const char *argv[]) {
    if (argc < 6) {
        cout << "Wrong number of parameters" << endl;
        cout << "Usage:.\\GA_TS <population-size> <new-at-generation> "
                "<time-limit> <instance-file-name> <result-file-name>" << endl;
        exit(0);
    } else {
        srand(time(NULL));
        srand48(time(NULL));
        srandom(time(NULL));
        int time = 0, populationSize = 0, newIndividuals = 0;
        double solution = 0;

        stringstream convertTime(argv[3]), convertPopSize(argv[1]), convertNewInd(argv[2]);

        convertTime >> time;
        convertPopSize >> populationSize;
        convertNewInd >> newIndividuals;

        auto graph = new Graph(argv[4]);

        solution = hybridGeneticAlgorithm(graph, time, populationSize, newIndividuals, argv[5]);

        cout << "The fitness of the fittest individual was " << solution << endl;
    }
    return 0;
}