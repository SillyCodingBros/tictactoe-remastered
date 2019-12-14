#include "IComputer.hpp"

// Création d'un objet IComputer
IComputer::IComputer(symbole signe, Board* board) : Player(signe,board){
}

// Jouer un coup
bool IComputer::play(){
  int grid, cell;
  algorithm(grid,cell);
  std::cout << "AI: I played in " << grid+1 << ", " << cell+1 << '\n';
  return board_->update(symbole_,grid,cell);
}
