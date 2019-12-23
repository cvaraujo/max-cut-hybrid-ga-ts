//
// Created by carlos on 26/04/18.
//

#include "../include/Algorithm.h"


void Algorithm::defaultEvolvePopulation(Population *population, int newInd) {
    for (int i = 0; i < newInd; i++) {
        population->appendIndividual(defaultCrossover(tournament(population), tournament(population)));
        population->setIndividual(population->getSize() - 1,
                                  population->getIndividual(population->getSize() - 1)->TS(10e3));
    }

    population->setIndividual(0, population->getFittest());
    population->getOnly(population->getSize(), newInd);
}

Individual *Algorithm::defaultCrossover(Individual *individual1, Individual *individual2) {
    auto *child = new Individual((individual1->getFitness() > individual2->getFitness()) ? *individual1 : *individual2);

    for (int i = 0; i < individual1->getSize(); i++) {
        if (child->getGene(i) == individual2->getGene(i)) continue;
        else if (drand48() < uniformRate) child->setGene(i);
    }
    return child;
}

Individual *Algorithm::tournament(Population *population) {
    auto *individual = new Individual(nullptr, false, false);
    int selected = 0;

    for (int i = 0; i < tournamentSize; i++) {
        selected = int(random() % population->getSize());
        if (individual->getFitness() < population->getIndividual(selected)->getFitness()) {
            individual = population->getIndividual(selected);
        }
    }

    return individual;
}
