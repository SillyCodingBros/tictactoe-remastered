#include "Player.hpp"

Player::Player(symbole signe, Board* board) : symbole_(signe), board_(board){
}

symbole Player::getSymbole(){
  return symbole_;
}


User::User(symbole signe, Board* board) : Player(signe, board){
}

bool User::play(){
  int gridInput, cellInput;
  std::cout << "player " << symbole_ << " plz enter a num_grid and a num_cell" << '\n';
  std::cin >> gridInput >> cellInput;
  while (gridInput < 0 || gridInput > 8 || cellInput < 0 || cellInput > 8) {
    std::cout << "bad input" << '\n';
    std::cout << "player " << symbole_ << " plz enter a num_grid and a num_cell" << '\n';
    std::cin >> gridInput >> cellInput;
  }
  return board_->update(symbole_,gridInput,cellInput);
}


IComputer::IComputer(symbole signe, Board* board) : Player(signe,board){
}

bool IComputer::play(){
  int grid, cell;
  heuristic(std::ref(grid),std::ref(cell));
  std::cout << "debug : IA played in " << grid << ", " << cell << '\n';
  return board_->update(symbole_,grid,cell);
}

AuPif::AuPif(symbole signe, Board* board) : IComputer(signe, board){
  std::srand(std::time(nullptr));
}

void AuPif::heuristic(int& grid, int& cell){
  grid = board_->getCurGrid();
  if (board_->fullGrid(grid)) {
    while (grid == board_->getCurGrid()){
      grid = std::rand()/((RAND_MAX + 1u)/9);
    }
  }
  cell = std::rand()/((RAND_MAX + 1u)/9);
}
