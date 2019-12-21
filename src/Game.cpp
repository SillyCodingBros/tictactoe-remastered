#include "Game.hpp"

// Création des joueurs selon le mode de jeu
Game::Game(gamemode mode){
  int nbThread, depth;
  board_ = Board();
  switch (mode) {
    case PVP:
      std::cout << "~~~ Player vs Player ~~~" << '\n';
      players_[0] = new User(CROSS, board_);
      players_[1] = new User(CIRCLE, board_);
      std::cout << "Player 1 plays with \"X\"\n"
                << "Player 2 plays with \"O\"\n" << '\n';
      board_.draw();
      break;
    case PVRANDOM:
      std::cout << "\n~~~ Player vs RANDOM ~~~\n" << '\n';
      players_[0] = new User(CROSS, board_);
      players_[1] = new Random(CIRCLE, board_);
      std::cout << "You, player 1 will use \"X\"\n"
                << "Random plays with \"O\"\n" << '\n';
      board_.draw();
      break;
    case PVAI:
      settings(nbThread, depth);
      std::cout << "\n~~~ Player vs AI ~~~\n" << '\n';
      players_[0] = new User(CROSS, board_);
      players_[1] = new MinMax(CIRCLE, CROSS, board_,        nbThread, depth);
      std::cout << "You, player 1 will use \"X\"\n"
                << "AI plays with \"O\"\n" << '\n';
      board_.draw();
      break;
    case AIVAI:
      std::cout << "\n~~~ AI vs AI ~~~\n" << '\n';
      std::cout << "Settings AI 1" << '\n';
      settings(nbThread, depth);
      players_[0] = new MinMax(CROSS, CIRCLE, board_,        nbThread, depth);
      std::cout << "Settings AI 2" << '\n';
      settings(nbThread, depth);
      players_[1] = new MinMax(CIRCLE, CROSS, board_,        nbThread, depth);
      std::cout << "AI 1 plays with \"X\"\n"
                << "AI 2 plays with \"O\"\n" << '\n';
      break;
    case RANDOMVAI:
      settings(nbThread, depth);
      std::cout << "\n~~~ Random vs AI ~~~\n" << '\n';
      players_[0] = new Random(CROSS, board_);
      players_[1] = new MinMax(CIRCLE, CROSS, board_,        nbThread, depth);
      std::cout << "Ramdom plays with \"X\"\n"
                << "AI plays with \"O\"\n" << '\n';
      break;
    case PVBOBAI:
      settings(nbThread, depth);
      std::cout << "\nPvAI\n" << '\n';
      players_[0] = new User(CROSS, board_);
      players_[1] = new BobbyMinMax(CIRCLE, CROSS, board_,    nbThread, depth);
      std::cout << "You, player 1 will use \"X\"\n"
                << "AI plays with \"O\"\n" << '\n';
      board_.draw();
      break;
    case BOBAIVBOBAI:
      std::cout << "\nAIvAI\n" << '\n';
      std::cout << "Settings AI 1" << '\n';
      settings(nbThread, depth);
      players_[0] = new BobbyMinMax(CROSS, CIRCLE, board_,    nbThread, depth);
      std::cout << "Settings AI 2" << '\n';
      settings(nbThread, depth);
      players_[1] = new BobbyMinMax(CIRCLE, CROSS, board_,    nbThread, depth);
      std::cout << "AI 1 plays with \"X\"\n"
                << "AI 2 plays with \"O\"\n" << '\n';
      break;
    case RANDOMVBOBAI:
      settings(nbThread, depth);
      std::cout << "\nRANDOMvAI\n" << '\n';
      players_[0] = new Random(CROSS, board_);
      players_[1] = new BobbyMinMax(CIRCLE, CROSS, board_,    nbThread, depth);
      std::cout << "RANDOM 1 plays with \"X\"\n"
                << "AI 2 plays with \"O\"\n" << '\n';
      break;
    default:
      std::cerr << "Error wrong gamemode : \'" << mode << "\'" << '\n';
      break;
  }
}

// Paramètres pour les IA (nombre de threads et profondeur)
void Game::settings(int& nbThread, int& depth){
  char choice;
  int nbThread_(4), depth_(6);
  std::cout << "Default values :\n\t4 threads\n\t6 of depth\n" <<
            "Do you want to set thread and depth values (y/n) : ";
  std::cin >> choice;
  if (choice == 'y') {
    std::cout << "Values for thread number and depth\n" <<
                  "Enter (number, number) : ";
    std::cin >> nbThread_ >> depth_;
    while (nbThread_ < 1 || depth_ < 1) {
      std::cout << "Values must be  > 1\n" <<
                  "Enter (number, number) : ";
      std::cin >> nbThread_ >> depth_;
    }
  }
  nbThread = nbThread_;
  depth = depth_;
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
