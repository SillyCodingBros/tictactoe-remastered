#include "MinMax.hpp"

// Création d'un objet MinMax
MinMax::MinMax(symbole signe, symbole opponent, Board* board, int nbThread, int depth)
              : IComputer(signe, board), opponent_(opponent), depth_(depth){
}

MinMax::Node::Node(Node* parent, int value, int nbChild, int cell, int grid) : parent_(parent), value_(value), nbChild_(nbChild), cell_(cell), grid_(grid){
}

MinMax::Node* MinMax::Node::updateMe(int value, int cell, int grid){
  Node* result = nullptr;
  //lock
  //lock parent && grandParent
  if (!parent_->parent_->test(value)) {
    parent_->nbChild_ = 0;
    parent_->parent_->nbChild_ -=1;
  }
  //unlock parent && grandParent
  if (chemin()) {
    result = parent_->updateMe(value,cell_,grid_);
  }else{
    nbChild_ -=1;
    if (test(value)) {
      value_ = value;
      result = this;
      if (parent_ == nullptr) {
        cell_ = cell;
        grid_ = grid;
      }
    }
  }
  //unlock
  return result;
}

bool MinMax::Node::chemin(){
  Node* tmp = this;
  while (tmp != nullptr) {
    if (tmp->nbChild_ == 0) {
      return false;
    }
  }
  return true;
}

MinMax::Min::Min(Node* parent, int nbChild, int cell, int grid) : Node(parent,INT_MAX,nbChild,cell,grid){
}

bool MinMax::Min::test(int value){
  return value_ < value;
}

MinMax::Max::Max(Node* parent, int nbChild, int cell, int grid) : Node(parent,INT_MIN,nbChild,cell,grid){
}

bool MinMax::Max::test(int value){
  return value_ > value;
}

// Algorithme de choix de coup minmax
void MinMax::algorithm(int& grid, int& cell) {
  Board tmp = *board_;
}

// Heuristique de l'algorithme
int MinMax::heuristic(Board& board){
  int sum = 0;
  for (auto i=0; i < 9; ++i){
    for (auto j=0; j < 8; ++j){
      sum += evaluateLine(caseValue(board.getCell(POSSIBILITIES_[0 + (j * 3)] + (9 * i))) +
                           caseValue(board.getCell(POSSIBILITIES_[1 + (j * 3)] + (9 * i))) +
                           caseValue(board.getCell(POSSIBILITIES_[2 + (j * 3)] + (9 * i))));
    }
  }
  return sum;
}


// Retourne le poids associé à une ligne*
//*(Somme des valeurs associées aux symboles qui la compose)
int MinMax::evaluateLine(int line){
  if (line == -1){
    return 0;
  }
  else if (line == 3){
    return maxValue_;
  }
  else if (line == -6){
    return minValue_;
  }
  else if (line == -3){
    return maxValue_/2;
  }
  else if (line < 0){
    return line/2;
  }
  return line;
}


// Associe une valeur à un symbole
int MinMax::caseValue(symbole cell){
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

/*
  /!\ Fonction PARFAITE /!\
  Retourne la liste des coups possibles pour une grille donnée
*/
void MinMax::possibleMove(Board& board, std::vector<std::pair<int,int>>& move){
  int curGrid = board.getCurGrid();

  if (curGrid == -1) {
    for(auto i = 0; i < 81; ++i) {
      if (board.getCell(i) == NOTHING) {
        move.emplace_back((i / 9), (i % 9));
      }
    }
  }
  else {
    for (auto i = 0; i < 9; ++i){
      if (board.getCell((curGrid * 9) + i) == NOTHING){
        move.emplace_back(curGrid, i);
      }
    }
  }
}
