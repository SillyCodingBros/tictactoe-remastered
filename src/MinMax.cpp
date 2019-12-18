#include "MinMax.hpp"

// Création d'un objet MinMax
MinMax::MinMax(symbole signe, symbole opponent, Board* board, int nbThread, int depth)
              : IComputer(signe, board), opponent_(opponent), depth_(depth), nbThread_(nbThread){
}

/* constructeur de Node */
MinMax::Node::Node(Node* parent, int value, int cell, int grid) : parent_(parent), value_(value), cell_(cell), grid_(grid){
}

/* Fonction pour update le node avec une nouvelle valeur */
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
      if (parent_ == nullptr) {
        cell_ = cell;
        grid_ = grid;
      }
      result = this;
    }
  }
  //unlock
  return result;
}

/* fonction qui test si la branche actuelle est élagué */
bool MinMax::Node::chemin(){
  Node* tmp = this;
  while (tmp != nullptr) {
    if (tmp->nbChild_ == 0) {
      return false;
    }
  }
  return true;
}

/* setter de la variable nbChild_ */
void MinMax::Node::setNbChild(int nbChild){
  nbChild_ = nbChild;
}

/* getter de la variable nbChild_ */
int MinMax::Node::getNbChild(){
  return nbChild_;
}

/* getter de la variable value_ */
int MinMax::Node::getValue(){
  return value_;
}

/* getter de la variable cell_ */
int MinMax::Node::getCell(){
  return cell_;
}

/* getter de la variable grid_ */
int MinMax::Node::getGrid(){
  return grid_;
}

/* constructeur de Min */
MinMax::Min::Min(Node* parent, int cell, int grid) : Node(parent,INT_MAX,cell,grid){
}

/* test utiliser par Min */
bool MinMax::Min::test(int value){
  return value_ < value;
}

/* constructeur de Max */
MinMax::Max::Max(Node* parent, int cell, int grid) : Node(parent,INT_MIN,cell,grid){
}

/* test utiliser par Max */
bool MinMax::Max::test(int value){
  return value_ > value;
}

// Algorithme de choix de coup minmax
void MinMax::algorithm(int& grid, int& cell) {
  std::vector<std::thread> listThread;
  Board tmp = *board_;
  Max origin(nullptr,0,0);
  createNode(tmp,depth_,&origin);
  for (int i = 0; i < nbThread_; i++) {
    listThread.emplace_back(&MinMax::funcThread, this);
  }
  while (origin.getNbChild() == 0) {
  }
  grid = origin.getGrid();
  cell = origin.getCell();
}

/* Fonction des threads */
void MinMax::funcThread() {
  std::function<void()> task;

  while (!taskQueue_.empty()) {
    //lock queue
    task = taskQueue_.front();
    taskQueue_.pop();
    //unlock queue
    task();
  }
}

/* Fonction de creation de Node destiner a la queue de tache a faire */
void MinMax::createNode(const Board& board, int depth, Node* parent){
  std::vector<std::pair<int,int>> move;
  Board tmp = board;
  int newDepth = depth - 1;
  possibleMove(tmp,move);
  parent->setNbChild(move.size());
  for (size_t i = 0; i < move.size(); i++) {
    tmp = board;
    tmp.update(symbole_,move[i].first,move[i].second);
    if (newDepth == 0 || tmp.gameState() != NOTHING) {
      parent->updateMe(move[i].first,move[i].second,heuristic(tmp));
    }else{
      //lock
      taskQueue_.push([this,tmp,newDepth,parent] {
        Node* node;
        if (parent->isMax()) {
          node = new Min(parent,parent->getCell(),parent->getGrid());
        }else{
          node = new Max(parent,parent->getCell(),parent->getGrid());
        }
        this->createNode(tmp,newDepth,node);
      });
      //unlock
    }
  }
}

/* Fonction pour update les Node destiner a la queue de tache a faire */
void MinMax::updateNode(Node* curNode){
  curNode = curNode->updateMe(curNode->getValue(), curNode->getCell(), curNode->getGrid());
  //push fonction new curNode
  taskQueue_.push([this,curNode] {
    this->updateNode(curNode);
  });
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
