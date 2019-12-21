#ifndef MINMAX_HPP
#define MINMAX_HPP

#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <vector>
#include <climits>
#include <chrono>

#include "IComputer.hpp"

class BobbyMinMax : public IComputer {
private:

  class Node {
  protected:
    Node* parent_;
    int value_;
    int nbChild_;
    int grid_;
    int cell_;

    std::mutex nodeMutex_;
  public:
    Node (Node* parent, int value, int grid, int cell);
    void updateMe(int value, int grid, int cell);
    virtual bool test(int value) = 0;
    virtual bool isMax() = 0;
    Node* alphaBeta();
    bool chemin();
    void setNbChild(int nbChild);
    int getNbChild();
    int getValue();
    int getCell();
    int getGrid();
  };

  class Min : public Node{
  public:
    Min (Node* parent, int grid, int cell);
    bool test(int value);
    bool isMax();
  };

  class Max : public Node{
  public:
    Max (Node* parent, int grid, int cell);
    bool test(int value);
    bool isMax();
  };

  /* pour minmax */
  symbole opponent_;
  int depth_;

  /* les threads */
  int nbThread_;
  bool job_;

  /* pour les threads */
  std::queue<std::function<void()>> taskQueue_;
  std::mutex taskMutex_;

  /* variable pour le calcule de l'heuristic */
  int maxValue_ = 50;
  int minValue_ = -50;
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
  void funcThread();
  void createNode(const Board& board, int depth, Node* parent);

  int evaluateLine(int line);
  int caseValue(symbole cell);

  void possibleMove(Board& board, std::vector<std::pair<int,int>>& move);

public:

  BobbyMinMax(symbole signe, symbole opponent, Board* board, int nbThread, int depth);
};

#endif /* end of include guard: MINMAX_HPP */
