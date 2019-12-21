#ifndef GAME
#define GAME

#include <iostream>
#include <typeinfo>
#include <chrono>

#include "Board.hpp"
#include "Player.hpp"
#include "Random.hpp"
#include "MinMax.hpp"
#include "BobbyMinMax.hpp"
#include "type.hpp"

class Game {
private:
  Player* players_[2];
  Board board_;

  void settings(int& nbThread, int& depth);

public:
  Game(gamemode mode);
  void launch();
};

#endif /* end of include guard: GAME */
