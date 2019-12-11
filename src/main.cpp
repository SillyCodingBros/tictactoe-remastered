#include "Game.hpp"

// Affichage du menu et lancement de la boucle de jeu
int main(int argc, char const *argv[]) {
  int gameId;

  std::cout << "Choose your opponent:\n\t1 - Local player\n\t2 - Random\n\t3 - AI\n\nEnter the number: ";
  std::cin >> gameId;
  while (gameId < 1 || gameId > 3) {
    std::cout << "Wrong number\n";
    std::cout << "Choose your opponent:\n\t1 - Local player\n\t2 - Random\n\t3 - AI\n\nEnter the number: ";
    std::cin >> gameId;
  }

  Game game(gamemode (gameId-1));
  game.launch();
  return 0;
}
