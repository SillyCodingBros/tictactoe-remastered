#include "Game.hpp"

// Affichage du menu et lancement de la boucle de jeu
int main(int argc, char const *argv[]) {
  int gameId;

  std::cout << "Choose your opponent:\n\t" <<
                "1 - Local player\n\t"<<
                "2 - Player vs Random\n\t"<<
                "3 - Player vs AI\n\t"<<
                "4 - AI vs AI\n\t"<<
                "5 - Random vs AI\n\t"<<
                "6 - Player vs BobbyAI\n\t"<<
                "7 - BobbyAI vs BobbyAI\n\t"<<
                "8 - Random vs BobbyAI\n"<<
                "\nEnter the number: ";
  std::cin >> gameId;
  while (gameId < 1 || gameId > 8) {
    std::cout << "Wrong number\n";
    std::cout << "Choose your opponent:\n\t" <<
                  "1 - Local player\n\t"<<
                  "2 - Player vs Random\n\t"<<
                  "3 - Player vs AI\n\t"<<
                  "4 - AI vs AI\n\t"<<
                  "5 - Random vs AI\n\t"<<
                  "6 - Player vs BobbyAI\n\t"<<
                  "7 - BobbyAI vs BobbyAI\n\t"<<
                  "8 - Random vs BobbyAI\n"<<
                  "\nEnter the number: ";
    std::cin >> gameId;
  }

  Game game(gamemode (gameId-1));
  game.launch();
  return 0;
}
