#include "Game.hpp"

Game::Game(gamemode mode){
  board_ = Board();
  switch (mode) {
    case PVP:
      std::cout << "PVP" << '\n';
      players_[0] = new User(CROSS, &board_);
      players_[1] = new User(CIRCLE, &board_);
      break;
    case PVAUPIF:
      std::cout << "PVC" << '\n';
      players_[0] = new User(CROSS, &board_);
      players_[1] = new Random(CIRCLE, &board_);
      break;
    default:
      std::cerr << "error in gamemode" << '\n';
      break;
  }
}

void Game::launch(){
  bool game = true;
  int playerTurn = 0;

  while (game) {
    if (!players_[playerTurn]->play()) {
      std::cout << "you can't play there" << '\n';
      continue ;
    }
    if (board_.getWinner() != NOTHING) {
      game = false;
    }
    board_.draw();
    playerTurn = (playerTurn+1)%2;
  }
  std::cout << "player " << board_.getWinner() << " win the game !!" << '\n';
}
