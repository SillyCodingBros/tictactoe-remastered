#ifndef MINMAX_HPP
#define MINMAX_HPP

#include <thread>
#include <mutex>
#include <climits>
#include <vector>
#include <list>
#include <functional>
#include <chrono>
#include <cassert>

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

    void update(int value, int grid, int cell);

    Node(int min);
    Node(Node* parent, int value, bool max, int grid, int cell);
  };

public :

  bool game_;

  bool end_;
  std::mutex endMutex_;

  Node* origin_;

  symbole opponent_;
  int nbThread_;
  int depth_;

  std::vector<std::thread> threads_;
  std::list<std::function<void()>> taskQueue_;
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
  void funcThread();

  int heuristic(Board& board);
  int evaluateLine(int line);
  int caseValue(symbole cell);

  void createChildren(Node* parent, const Board& board, int depth);

  void possibleMoves(Board& board, std::vector<std::pair<int,int>>& moves);

  void pushTask(std::function<void()> task, bool place);
  void completeATask();

  symbole getSymbole();

  MinMax(symbole signe, symbole opponent, Board& board, int nbThread, int depth);

};

#endif /* end of include guard: MINMAX_HPP */
