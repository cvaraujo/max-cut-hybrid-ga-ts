//
// Created by carlos on 26/04/18.
//

#include "../include/Algorithm.h"


void Algorithm::defaultEvolvePopulation(Population *population, int newInd) {
  bool push;

  for (int i = 0; i < newInd; i++) {
    Individual *ind = multiParentCrossover(population, 2 + int(random() % 3));
    ind->TS(10e6);
    push = true;

    for (i = 0; i < population->getSize(); i++) {
      if (population->getIndividual(i)->getFitness() == ind->getFitness()) {
        push = false;
        break;
      }
    }

    if(push) population->appendIndividual(ind);
  }

    population->getOnly(population->getSize(), newInd);
}

Individual *Algorithm::multiParentCrossover(Population *population, int p) {
  vector<vector<int>> sideZero = vector<vector<int>>(p, vector<int>());
  vector<vector<int>> sideOne = vector<vector<int>>(p, vector<int>());
  int i, j, k, n = population->getIndividual(0)->getSize();
  vector<int> selecteds = vector<int>();
  vector<bool> allocated = vector<bool>(n);
  int first = 1 + int(random() % int(population->getSize() * 0.1));

  selecteds.push_back(first);
  selecteds.push_back(population->getSize()-1);

  for (k = first+1; k < p-1; k++)
    selecteds.push_back(k);

  for (i = 0; i < n; i++)
    for (k = 0; k < p; k++)
      if (population->getIndividual(selecteds[k])->getGene(i)) sideOne[k].push_back(i);
      else sideZero[k].push_back(i);

  int smallest = 0, smallestZero = 0, sizeSmallest = sideOne[0].size(), sizeSmallestZero = sideZero[0].size();

  for (k = 0; k < p; k++) {
    sort(sideZero[k].begin(), sideZero[k].end());
    sort(sideOne[k].begin(), sideOne[k].end());
    if (sideOne[k].size() < sizeSmallest) smallest = k, sizeSmallest = sideOne[k].size();
    if (sideZero[k].size() < sizeSmallestZero) smallestZero = k, sizeSmallestZero = sideZero[k].size();
  }

  Individual *ind = new Individual(population->getIndividual(smallest)->graph, false, false);

  bool change;
  for (i = 0; i < sizeSmallest; i++) {
    j = sideOne[smallest][i];
    change = true;
    for (k = 0; k < p; k++) {
      if (sideOne[k][i] != j) {
        change = false; break;
      }
    }
    if (change) ind->setGene(j), allocated[j] = true;
  }

  for (i = 0; i < sizeSmallestZero; i++) {
    j = sideZero[smallestZero][i];
    change = true;
    for (k = 0; k < p; k++) {
      if (sideZero[k][i] != j) {
        change = false; break;
      }
    }
    if (change) allocated[j] = true;
  }

  for (i = 0; i < n; i++)
    if (!allocated[j] && drand48() < 0.6) ind->setGene(i);

  return ind;
}


void Algorithm::defaultMutation(Individual *individual) {
    for (int i = 0; i < individual->getSize(); i++)
        if (drand48() < mutationRate) individual->setGene(i);
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
