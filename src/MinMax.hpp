#ifndef MINMAX_HPP
#define MINMAX_HPP

#include <thread>
#include <mutex>
//#include <queue>
#include <list>
#include <vector>
#include <utility>
#include <climits>
#include <cassert>
#include <chrono>

#include "IComputer.hpp"

class MinMax : public IComputer {
private:

  //classe pour les noeuds de l'arbre minmax
  class node {
  private:
    int grid_;
    int cell_;
    int workChild_;
    int value_;
    bool max_;
    std::mutex nodeMutex_;
    node* parent_;

  public:

    node();
    node(int grid, int cell, node* parent, int max, int min);
    void loadOrigin(int value);
    void submitWork();
    void setValue(int value);
    int getValue();
    bool isOrigin();
    int getParentWorkChild();
    int getWorkChild();
    void setWorkChild(int workChild);
    bool getMax();
    int getGrid();
    int getCell();
    void setGrid(int grid);
    node* getParent();
  };

  //classe pour envoyer les données requisent aux taches des threads
  class task {
  public:
    Board board_;
    node* curNode_;
    int depth_;
    bool up_;

    task();
    task(Board& board, node* curNode, int depth, bool up);
  };

  std::mutex print;

  symbole opponent_;
  bool game_;
  node* origin_;
  bool end_;
  int depth_;
  std::list<task> taskQueue_;
  std::mutex taskMutex_;

  std::vector<std::thread> listThread_;

  int maxValue_ = INT_MAX;
  int minValue_ = INT_MIN;
  int POSSIBILITIES_[24] = {0, 1, 2,
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
  void funcThread();
  void downTree(task& task);
  void upTree(task& task);
  void evaluate(task& task);
  int heuristic(Board& board);
  int evaluateLine(int line);
  int caseValue(symbole cell);


  void handleSubmitedWork(task& task);
  void handleTerminalNode(task& task);
  void handleNode(task& task);
  void possibleMove(Board& board, std::vector<std::pair<int,int>>& move);

  void branchNode(task& task);
  void printQueue();

public:

  MinMax(symbole signe, symbole opponent, Board* board, int nbThread, int depth);
};

#endif /* end of include guard: MINMAX_HPP */
