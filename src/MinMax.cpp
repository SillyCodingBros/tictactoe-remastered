#include "MinMax.hpp"

MinMax::MinMax(symbole signe, Board* board) : IComputer(signe, board){
  //creation des threads
  std::srand(std::time(nullptr));
  printf("Heuristic for this board : %d\n", heuristic(board));
}

void MinMax::algorithm(int& grid, int& cell) {
  grid = board_->getCurGrid();
  if (board_->fullGrid(grid)) {
    while (grid == board_->getCurGrid()){
      grid = std::rand()/((RAND_MAX + 1u)/9);
    }
  }
  cell = std::rand()/((RAND_MAX + 1u)/9);
  printf("Heuristic for this board : %d\n", heuristic(board_));
}

int MinMax::heuristic(Board* board){
  int sum = 0;
  for (auto i=0; i < 9; ++i){
    for (auto j=0; j < 8; ++j){
      sum += evaluate_line(case_value(board->getCell(POSSIBILITIES[0 + (j * 3)] + (9 * i))) +
                           case_value(board->getCell(POSSIBILITIES[1 + (j * 3)] + (9 * i))) +
                           case_value(board->getCell(POSSIBILITIES[2 + (j * 3)] + (9 * i))));
    }
  }
  return sum;
}

int MinMax::evaluate_line(int line){
  //printf("line = %d\n", line);
  if (line == -1){
    return 0;
  }
  else if (line == 3){
    return MAX_VALUE;
  }
  else if (line == -3){
    return 3;
  }
  else if (line < 0){
    return 0;
  }
  return line;
}

int MinMax::case_value(symbole cell){
  //printf("symbole = \'%d\'\n", cell);
  if (cell == getSymbole()) {
    return 1;
  }
  if (cell == NOTHING) {
    return 0;
  }
  else {
    return -2;
  }
}

void MinMax::funcThread(){
  //fonction pour les threads

  //boucle tant que la partie existe
  //attendre les taches a faire depuis la pile
  //get une tache puis la suppr
  //si feuille alors heuristic sinon min/max resultat des enfants
  //mettre le resultat de la tache dans l'emplacement memoire dedié
}
