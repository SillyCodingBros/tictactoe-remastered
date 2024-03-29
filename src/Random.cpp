#include "Random.hpp"

// Création de l'objet Random
Random::Random(symbole signe, Board& board) : IComputer(signe, board){
  std::srand(std::time(nullptr));
}

// Algorithme de choix de coup aléatoire
void Random::algorithm(int& grid, int& cell){
  grid = board_.getCurGrid();
  if (board_.fullGrid(grid) || grid == -1) {
    while (grid == board_.getCurGrid()){
      grid = std::rand()/((RAND_MAX + 1u)/9);
    }
  }
  cell = std::rand()/((RAND_MAX + 1u)/9);
}
