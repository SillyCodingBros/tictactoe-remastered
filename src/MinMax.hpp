#ifndef MINMAX_HPP
#define MINMAX_HPP

#include <thread>
#include <mutex>
#include <climits>
#include <vector>
#include <queue>
#include <functional>
#include <chrono>
#include <cassert>
#include <memory>

#include "IComputer.hpp"

class MinMax : public IComputer{
private:

  class Node {
  public:

    Node* parent_;
    int value_;
    int nbChild_;
    bool max_;
    int grid_;
    int cell_;

    std::mutex nodeMutex_;

  //public:
    void update(int value, int grid, int cell);
    //void createChildren(Node* parent, Board& board, int depth);

  //public:
    int getGrid();
    int getCell();
    Node(int min);
    Node(Node* parent, int value, bool max, int grid, int cell);
  };

public :

  bool end_;
  bool game_;
  std::mutex endMutex_;

  std::mutex originMutex_;
  Node* origin_;

  symbole opponent_;
  int nbThread_;
  int depth_;

  std::vector<std::thread> threads_;
  std::queue<std::function<void()>> taskQueue_;
  std::mutex taskMutex_;

  int INFINITE_MAX = INT_MAX;
  int INFINITE_MIN = INT_MIN;

  int POSSIBILITIES_[24] = {0, 1, 2,
                           3, 4, 5,
                           6, 7, 8,
                           0, 3, 6,
                           1, 4, 7,
                           2, 5, 8,
                           0, 4, 8,
                           2, 4, 6};

  void algorithm(int& grid, int& cell);

  void handleThreads();

  int heuristic(Board& board);
  int evaluateLine(int line);
  int caseValue(symbole cell);

  void createChildren(Node* parent, const Board& board, int depth);

  void possibleMoves(Board& board, std::vector<std::pair<int,int>>& moves);

  void pushTask(std::function<void()> task);
  void completeATask();

  symbole getSymbole();

//public:

  MinMax(symbole signe, symbole opponent, Board& board, int nbThread, int depth);

};

class FunctionForThread {
public:
  MinMax* minmax_;

  void funcThread();

  FunctionForThread(MinMax* minmax);

};


#endif /* end of include guard: MINMAX_HPP */
