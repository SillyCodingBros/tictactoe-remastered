#ifndef PLAYER
#define PLAYER

#include "type.hpp"
#include "Board.hpp"

class Player{
protected:
  symbole symbole_;
  Board* board_;

public:
  Player();
  Player(symbole signe, Board* board);
  bool play(int grid, int cell);
  symbole getSymbole();
};



class IComputer : public Player {
private:
  virtual void heuristic(int& grid, int& cell);

public:
  IComputer();
  bool play();
};

#endif /* end of include guard: PLAYER */
