//
// Created by carlos on 26/04/18.
//

#include "../include/Individual.h"

Individual::Individual(Graph *graph, bool initialSolution, bool isDefault) {
  if (graph != nullptr) {
    this->fitness = 0.0;
    this->graph = graph;
    int n = graph->getN();

    chg = vector<int>(n);
    side = vector<bool>(n);
    gene = vector<bool>(n);
    cx = vector<int>(n);

    for (int i = 0; i < graph->getN(); i++) {
      chg[i] = graph->getChg(i), gene[i] = false, side[i] = false, cx[i] = 0;
    }

    if (initialSolution) {
      if (isDefault) defaultSolution();
      else constrainedSolution();
    }
  } else {
    this->fitness = numeric_limits<int>::min();
  }
}

void Individual::constrainedSolution() {
  int i, j, n = graph->getN(), fixed = int(random() % n);
  double value = 0;
  bool hasChange;

  do {
    hasChange = false;

    for (i = 0; i < n; i++) {
      if (i != fixed) {
        if (cx[i] < (graph->getC(i) / 2) && gene[i] == 0) {
          // gene[i] have to be 1
          gene[i] = true, side[i] = true, hasChange = true;
          value += chg[i], chg[i] = -chg[i];
          for (auto e : graph->graph[i]) {
            j = e.first;
            cx[j] += e.second;
            chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
          }
        } else if (cx[i] > (graph->getC(i) / 2) && gene[i] == 1) {
          // gene[i] have to be 0
          gene[i] = false, side[i] = false, hasChange = true;
          value += chg[i], chg[i] = -chg[i];
          for (auto e : graph->graph[i]) {
            j = e.first;
            cx[j] -= e.second;
            chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
          }
        }
      }
    }
  } while (hasChange);

  this->fitness = value;
  //    checkConstraints();
}

void Individual::defaultSolution() {
  int n = graph->getN(), j;
  double value = 0.0;

  for (int i = 0; i < n; i++) {
    if (drand48() <= 0.5) {
      gene[i] = true, side[i] = true;
      value += chg[i], chg[i] = -chg[i];
      for (auto e : graph->graph[i]) {
        j = e.first;
        cx[j] += e.second;
        chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
      }
    }
  }
  this->fitness = value;
}

int Individual::getGene(int i) const {
  return this->gene[i];
}

void Individual::setGene(int i) {
  int j;
  gene[i] = !gene[i], side[i] = !side[i];
  fitness += chg[i], chg[i] = -chg[i];
  for (auto e : graph->graph[i]) {
    j = e.first;
    if (gene[i]) cx[j] += e.second;
    else cx[j] -= e.second;
    chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
  }
}

Individual *Individual::TS(int maxIter) {
  auto *incumbent = new Individual(nullptr, false, false);
  vector<int> p = vector<int>(15);
  vector<int> a = vector<int>(15);
  vector<int> interval = vector<int>(15);

  p = {1, 2, 1, 4, 1, 2, 1, 8, 1, 2, 1, 4, 1, 2, 1};
  interval[0] = 300;

  int last = 100, indexLast = 0;
  for (int i = 0; i < 15; i++) {
    a[i] = (0.125 * p[i] * 200);
    if (i != 14) interval[i + 1] = interval[i] + 4 * 100 * 0.125 * p[i];
  }

  int count = 0;
  makeTL(a[0]);

  for (int i = 0; i < maxIter; i++) {
    updateTL(i, last, indexLast, a, interval);
    neighborhoodMove();
    if (incumbent->getFitness() < fitness) {
      incumbent = new Individual(*this);
      count = 0;
    } else count++;
    if (count >= int(maxIter*0.001)) {
      count = 0;
      shuffleSolution();
    }
  }

  return incumbent;
}

void Individual::makeTL(int tenure) {
  TL = vector<int>();
  for (int i = 0; i < tenure; i++) TL.push_back(-1);
}

void Individual::updateTL(int i, int &last, int &indexLast, vector<int> &a, vector<int> &interval) {
  if (indexLast < 14) {
    if (i == last - 1) {
      last += interval[++indexLast];
      int newTLSize = a[indexLast];
      if (TL.size() < newTLSize)
        for (int j = TL.size(); j < newTLSize; j++) TL.push_back(-1);
      else TL.erase(TL.begin() + newTLSize, TL.end());
    }
  }

}

void Individual::updateCL() {
  CL = vector<int>();
  for (int i = 0; i < graph->getN(); i++)
    if ((cx[i] < (graph->getC(i) / 2) && gene[i] == 0) ||
        (cx[i] > (graph->getC(i) / 2) && gene[i] == 1) || chg[i] >= 0) {
      CL.push_back(i);
    }
}

void Individual::neighborhoodMove() {
  double maxDeltaCost = numeric_limits<int>::min();
  int bestCand = -1, deltaCost;
  vector<int>::iterator it;
  updateCL();

  random_shuffle(CL.begin(), CL.end());

  for (auto i : CL) {
    deltaCost = chg[i];
    it = find(TL.begin(), TL.end(), i);
    if (it != TL.end() || deltaCost >= 0) {
      if (maxDeltaCost < deltaCost) {
        bestCand = i;
        maxDeltaCost = deltaCost;
        break;
      }
    }
  }

  TL.erase(TL.begin());
  if (bestCand != -1) {
    TL.push_back(bestCand);
    it = find(CL.begin(), CL.end(), bestCand);
    CL.erase(it);
    setGene(bestCand);
  } else TL.push_back(-1);
}

void Individual::shuffleSolution() {
  int i, size = getSize();
  vector<pair<int, int>> order;

  for(i = 0; i < size; i++)
    order.push_back(make_pair(i, getChg(i)));

  sort(order.begin(), order.end(), [] (const pair<int, int> P1, const pair<int, int> P2) {
                                     return P1.second < P2.second;
                                   });

  for (i = 0; i < int(size * 0.15); i++)
    if(drand48() < 0.7) setGene(order[i].first);
}

double Individual::getFitness() const {
  return fitness;
}

int Individual::getSize() {
  return (int) this->gene.size();
}

int Individual::getCx(int i) {
  return cx[i];
}

int Individual::getChg(int i) {
  return chg[i];
}
