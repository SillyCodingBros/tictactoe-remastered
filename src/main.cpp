#include "Game.hpp"

// Affichage du menu et lancement de la boucle de jeu
int main(int argc, char const *argv[]) {
  int gameId;

  std::cout << "Choose your opponent:\n\t" <<
                "1 - Local player\n\t"<<
                "2 - Random\n\t"<<
                "3 - AI\n\t"<<
                "4 - AI vs AI\n"<<
                "\nEnter the number: ";
  std::cin >> gameId;
  while (gameId < 1 || gameId > 4) {
    std::cout << "Wrong number\n";
    std::cout << "Choose your opponent:\n\t" <<
                  "1 - Local player\n\t"<<
                  "2 - Random\n\t"<<
                  "3 - AI\n"<<
                  "4 - AI vs AI\n"<<
                  "\nEnter the number: ";
    std::cin >> gameId;
  }

  Game game(gamemode (gameId-1));
  game.launch();
  return 0;
}
