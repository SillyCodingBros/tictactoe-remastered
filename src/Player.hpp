#ifndef PLAYER
#define PLAYER

#include <functional>
#include <ctime>
#include <cstdlib>

#include "type.hpp"
#include "Board.hpp"

class Player{
protected:
  symbole symbole_;
  Board* board_;

public:
  Player(symbole signe, Board* board);
  virtual bool play() = 0;
  symbole getSymbole();
};

class User : public Player{
public:
  User(symbole signe, Board* board);
  bool play();
};

class IComputer : public Player {
private:
  virtual void heuristic(int& grid, int& cell) = 0;

public:
  IComputer(symbole signe, Board* board);
  bool play();
};


class AuPif : public IComputer {
private:
  void heuristic(int& grid, int& cell);

public:
  AuPif(symbole signe, Board* board);
};

#endif /* end of include guard: PLAYER */
