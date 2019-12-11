#include "Game.hpp"

// Création des joueurs selon le mode de jeu
Game::Game(gamemode mode){
  board_ = Board();
  switch (mode) {
    case PVP:
      std::cout << "PvP" << '\n';
      players_[0] = new User(CROSS, &board_);
      players_[1] = new User(CIRCLE, &board_);
      std::cout << "Player 1 plays with \"O\"\n"
                << "Player 2 plays with \"X\"" << '\n';
      break;
    case PVRANDOM:
      std::cout << "PvRANDOM" << '\n';
      players_[0] = new User(CROSS, &board_);
      players_[1] = new Random(CIRCLE, &board_);
      std::cout << "You, player 1 will use \"O\"\n"
                << "Random plays with \"X\"" << '\n';
      break;
    case PVAI:
      std::cout << "PvAI" << '\n';
      players_[0] = new User(CROSS, &board_);
      players_[1] = new MinMax(CIRCLE, &board_);
      std::cout << "You, player 1 will use \"O\"\n"
                << "AI plays with \"X\"" << '\n';
      break;
    default:
      std::cerr << "Error wrong gamemode : \'" << mode << "\'" << '\n';
      break;
  }
}

// Boucle de jeu
void Game::launch(){
  bool game = true;
  int playerTurn = 0;

  while (game) {
    if (!players_[playerTurn]->play()) {
      std::cout << "You can't play there" << '\n';
      continue ;
    }
    if (board_.getWinner() != NOTHING) {
      game = false;
    }
    board_.draw();
    playerTurn = (playerTurn+1)%2;
  }
  std::cout << "Player " << board_.getWinner() << " wins the game !!" << '\n';
}
