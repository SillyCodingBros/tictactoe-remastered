#ifndef MINMAX_HPP
#define MINMAX_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <utility>
#include <climits>
#include <cassert>
#include "IComputer.hpp"


class MinMax : public IComputer {
private:

  //classe pour les noeuds de l'arbre minmax
  class node {
  public:
    int grid;
    int cell;
    int workChild;
    int value;
    bool max;
    std::mutex nodeMutex;
    node* parent;

    node();
    node(int grid, int cell, int value, bool max, node* parent);
    void loadOrigin(int minValue);
  };

  //classe pour envoyer les données requisent aux taches des threads
  class task {
  public:
    Board board;
    node* curNode;
    int depth;
    bool up;

    task();
    task(Board board, node* curNode, int depth, bool up);
  };

  symbole opponent_;
  bool game;
  int depth;
  std::queue<task> taskQueue;
  std::mutex taskMutex;

  std::vector<std::thread> listThread;
  std::condition_variable cond_var;
  std::mutex mutex_cond_var;

  int maxValue = INT_MAX;
  int minValue = INT_MIN;
  int POSSIBILITIES[24] = {0, 1, 2,
                           3, 4, 5,
                           6, 7, 8,
                           0, 3, 6,
                           1, 4, 7,
                           2, 5, 8,
                           0, 4, 8,
                           2, 4, 6};

  void algorithm(int& grid, int& cell);
  void pushTask(task& task);
  void emplaceTask(Board& board, node* curNode, int depth, bool up);
  void funcThread(int id);
  int heuristic(Board* board);
  int evaluateLine(int line);
  int caseValue(symbole cell);
  void possibleMove(Board& board, std::vector<std::pair<int,int>>& move);

  std::mutex print;

public:
  MinMax(symbole signe, symbole opponent, Board* board, int nbThread, int depth);
};

#endif /* end of include guard: MINMAX_HPP */
