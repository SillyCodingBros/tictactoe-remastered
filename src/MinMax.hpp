#ifndef MINMAX_HPP
#define MINMAX_HPP

#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <vector>
#include <climits>

#include "IComputer.hpp"

class MinMax : public IComputer {
private:

  class Node {
  protected:
    int cell_;
    int grid_;
    Node* parent_;
    int value_;
    int nbChild_;
  public:
    Node (Node* parent, int value, int nbChild, int cell, int grid);
    Node* updateMe(int value, int cell, int grid);
    virtual bool test(int value) = 0;
    bool chemin();
  };

  class Min : Node{
  public:
    Min (Node* parent, int nbChild, int cell, int grid);
    bool test(int value);
  };

  class Max : Node{
  public:
    Max (Node* parent, int nbChild, int cell, int grid);
    bool test(int value);
  };

  /* pour minmax */
  symbole opponent_;
  int depth_;

  /* les threads */
  std::vector<std::thread> listThread_;

  /* pour les threads */
  std::queue<std::function<void()>> taskQueue_;
  std::mutex taskMutex_;

  /* variable pour le calcule de l'heuristic */
  int maxValue_ = 50;
  int minValue_ = 50;
  int POSSIBILITIES_[24] = {0, 1, 2,
                           3, 4, 5,
                           6, 7, 8,
                           0, 3, 6,
                           1, 4, 7,
                           2, 5, 8,
                           0, 4, 8,
                           2, 4, 6};

  void algorithm(int& grid, int& cell);
  int heuristic(Board& board);
  void createNode();
  void updateNode();

  int evaluateLine(int line);
  int caseValue(symbole cell);

  void possibleMove(Board& board, std::vector<std::pair<int,int>>& move);

public:

  MinMax(symbole signe, symbole opponent, Board* board, int nbThread, int depth);
};

#endif /* end of include guard: MINMAX_HPP */
