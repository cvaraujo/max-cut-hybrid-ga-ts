//
// Created by carlos on 26/04/18.
//

#ifndef PUREGENETICALGORITHM_GENE_H
#define PUREGENETICALGORITHM_GENE_H

#include "Graph.h"

class Individual {
    vector<int> chg, cx, CL, TL;
    vector<bool> side, gene;
    double fitness;

public:
    Graph *graph;

    Individual(Graph *graph, bool initialSolution, bool isDefault);

    void defaultSolution();

    void constrainedSolution();

    int getGene(int i) const;

    void setGene(int i);

    double getFitness() const;

    int getSize();

    int getCx(int i);

    int getChg(int i);

    // Tabu Search
    Individual *TS(int maxIter);

    void updateTL(int iter, int &last, int &indexLast, vector<int> &a, vector<int> &interval);

    void makeTL(int tenure);

    void updateCL();

    void shuffleSolution();

    void neighborhoodMove();

};


#endif //PUREGENETICALGORITHM_GENE_H
