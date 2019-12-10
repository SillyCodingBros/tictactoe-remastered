#include "IComputer.hpp"

IComputer::IComputer(symbole signe, Board* board) : Player(signe,board){
}

bool IComputer::play(){
  int grid, cell;
  algorithm(std::ref(grid),std::ref(cell));
  std::cout << "debug : IA played in " << grid << ", " << cell << '\n';
  return board_->update(symbole_,grid,cell);
}
