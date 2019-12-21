#include "Game.hpp"

// Création des joueurs selon le mode de jeu
Game::Game(gamemode mode){
  board_ = Board();
  switch (mode) {
    case PVP:
      std::cout << "PvP" << '\n';
      players_[0] = new User(CROSS, board_);
      players_[1] = new User(CIRCLE, board_);
      std::cout << "Player 1 plays with \"X\"\n"
                << "Player 2 plays with \"O\"\n" << '\n';
      board_.draw();
      break;
    case PVRANDOM:
      std::cout << "\nPvRANDOM\n" << '\n';
      players_[0] = new User(CROSS, board_);
      players_[1] = new Random(CIRCLE, board_);
      std::cout << "You, player 1 will use \"X\"\n"
                << "Random plays with \"O\"\n" << '\n';
      board_.draw();
      break;
    case PVAI:
      std::cout << "\nPvAI\n" << '\n';
      players_[0] = new User(CROSS, board_);
      players_[1] = new MinMax(CIRCLE, CROSS, board_,        4, 6);
      std::cout << "You, player 1 will use \"X\"\n"
                << "AI plays with \"O\"\n" << '\n';
      board_.draw();
      break;
    case AIVAI:
      std::cout << "\nAIvAI\n" << '\n';
      players_[0] = new MinMax(CROSS, CIRCLE, board_,        4, 3);
      players_[1] = new MinMax(CIRCLE, CROSS, board_,        4, 5);
      std::cout << "AI 1 plays with \"X\"\n"
                << "AI 2 plays with \"O\"\n" << '\n';
      break;
    case RANDOMVAI:
      std::cout << "\nRANDOMvAI\n" << '\n';
      players_[0] = new Random(CROSS, board_);
      players_[1] = new MinMax(CIRCLE, CROSS, board_,        4, 4);
      std::cout << "RANDOM 1 plays with \"X\"\n"
                << "AI 2 plays with \"O\"\n" << '\n';
      break;
    case PVBOBAI:
      std::cout << "\nPvAI\n" << '\n';
      players_[0] = new User(CROSS, board_);
      players_[1] = new BobbyMinMax(CIRCLE, CROSS, board_,    4, 6);
      std::cout << "You, player 1 will use \"X\"\n"
                << "AI plays with \"O\"\n" << '\n';
      board_.draw();
      break;
    case BOBAIVBOBAI:
      std::cout << "\nAIvAI\n" << '\n';
      players_[0] = new BobbyMinMax(CROSS, CIRCLE, board_,    4, 3);
      players_[1] = new BobbyMinMax(CIRCLE, CROSS, board_,    4, 5);
      std::cout << "AI 1 plays with \"X\"\n"
                << "AI 2 plays with \"O\"\n" << '\n';
      break;
    case RANDOMVBOBAI:
      std::cout << "\nRANDOMvAI\n" << '\n';
      players_[0] = new Random(CROSS, board_);
      players_[1] = new BobbyMinMax(CIRCLE, CROSS, board_,    4, 4);
      std::cout << "RANDOM 1 plays with \"X\"\n"
                << "AI 2 plays with \"O\"\n" << '\n';
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
    auto start = std::chrono::system_clock::now();

    if (!players_[playerTurn]->play()) {
      std::cout << "Player : "<< playerTurn << " You can't play there" << '\n';
      continue;
    }
    auto end = std::chrono::system_clock::now();
    auto timer = end - start;

    std::cout << "Thinking time : " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(timer).count()
    << " ms" << "\n\n";

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
