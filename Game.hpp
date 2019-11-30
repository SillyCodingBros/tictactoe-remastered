#ifndef GAME
#define GAME

#include <iostream>
#include <functional>

#include "Board.hpp"
#include "Player.hpp"
#include "type.hpp"

class Game {
private:
  Player players_[2];
  Board board_;

public:
  Game();
  void launch();
};

#endif /* end of include guard: GAME */
