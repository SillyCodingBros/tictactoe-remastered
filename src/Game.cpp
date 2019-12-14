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
      players_[1] = new MinMax(CIRCLE, CROSS, &board_, 1, 2);
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
      //continue ;
      break ;
    }
    if (board_.gameState() != NOTHING) {
      game = false;
    }
    board_.draw();
    playerTurn = (playerTurn+1)%2;
  }

  symbole state = board_.gameState();
  if (state != TIE) {
    std::cout << "Player " << state << " wins the game!" << '\n';
  }
  else{
    std::cout << "It's a tie! Try again to find out who's the master" << '\n';
  }
}
