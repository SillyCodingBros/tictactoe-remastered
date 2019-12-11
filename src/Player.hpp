#ifndef PLAYER
#define PLAYER

#include <functional>

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
private:
  void display(int* gridInput, int* cellInput, bool* cond);
public:
  User(symbole signe, Board* board);
  bool play();
};

#endif /* end of include guard: PLAYER */
