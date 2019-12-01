#include "Game.hpp"

Game::Game(){
  board_ = Board();
  players_[0] = Player(CROSS, &board_);
  players_[1] = Player(CIRCLE, &board_);
}

void Game::launch(){
  bool game = true;
  int playerTurn = 0;
  int gridInput, cellInput;

  while (game) {
    std::cout << "player " << players_[playerTurn].getSymbole() << " plz enter a num_grid and a num_cell" << '\n';
    std::cin >> gridInput >> cellInput;
    if (gridInput < 0 || gridInput > 8 || cellInput < 0 || cellInput > 8) {
      std::cout << "bad input" << '\n';
      continue;
    }
    if (!players_[playerTurn].play(gridInput, cellInput)) {
      std::cout << "you can't play in this cell" << '\n';
      continue;
    }
    if (board_.getWinner() != NOTHING) {
      game = false;
    }
    board_.draw();
    playerTurn = (playerTurn+1)%2;
  }
  std::cout << "player " << board_.getWinner() << " win the game !!" << '\n';
}
