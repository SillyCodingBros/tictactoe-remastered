#include "BobbyMinMax.hpp"

// Création d'un objet BobbyMinMax
BobbyMinMax::BobbyMinMax(symbole signe, symbole opponent, Board& board, int nbThread, int depth)
              : IComputer(signe, board), opponent_(opponent), depth_(depth), nbThread_(nbThread){
}

/* constructeur de Node */
BobbyMinMax::Node::Node(Node* parent, int value, int grid, int cell) : parent_(parent), value_(value), grid_(grid), cell_(cell){
}

/* Fonction pour update le node avec une nouvelle valeur */
void BobbyMinMax::Node::updateMe(int value, int grid, int cell){
  Node* nextUpdate = this;
  nodeMutex_.lock();
  if (chemin()) {
    if (test(value)) {
      value_ = value;
      if (parent_ == nullptr) {
        grid_ = grid;
        cell_ = cell;
      }
    }
    nbChild_ -= 1;
    nextUpdate = this;
    nodeMutex_.unlock();
    if (nextUpdate != nullptr) {
      if (nextUpdate->nbChild_ == 0 && nextUpdate->parent_ != nullptr) {
        nextUpdate->parent_->updateMe(nextUpdate->value_, nextUpdate->grid_, nextUpdate->cell_);
      }
    }
  }
  else{
    nodeMutex_.unlock();
  }
}

/* Fonction d'elaguage */
BobbyMinMax::Node* BobbyMinMax::Node::alphaBeta(){
  Node* result = nullptr;
  if (parent_ != nullptr) {
    result = this;
    parent_->nodeMutex_.lock();
    if (parent_->parent_ != nullptr) {
      parent_->parent_->nodeMutex_.lock();
      if (!parent_->parent_->test(value_)) {
        parent_->parent_->nbChild_ -=1;
        parent_->nbChild_ = 0;
        result = parent_->parent_;
      }
      parent_->parent_->nodeMutex_.unlock();
    }
    parent_->nodeMutex_.unlock();
  }
  return result;
}

/* fonction qui test si la branche actuelle est élagué */
bool BobbyMinMax::Node::chemin(){
  Node* tmp = parent_;
  while (tmp != nullptr) {
    if (tmp->nbChild_ == 0) {
      return false;
    }
    tmp = tmp->parent_;
  }
  return true;
}

/* setter de la variable nbChild_ */
void BobbyMinMax::Node::setNbChild(int nbChild){
  nbChild_ = nbChild;
}

/* getter de la variable nbChild_ */
int BobbyMinMax::Node::getNbChild(){
  return nbChild_;
}

/* getter de la variable value_ */
int BobbyMinMax::Node::getValue(){
  return value_;
}

/* getter de la variable cell_ */
int BobbyMinMax::Node::getCell(){
  return cell_;
}

/* getter de la variable grid_ */
int BobbyMinMax::Node::getGrid(){
  return grid_;
}

/* constructeur de Min */
BobbyMinMax::Min::Min(Node* parent, int grid, int cell) : Node(parent,INT_MAX,grid,cell){
}

/* test utiliser par Min */
bool BobbyMinMax::Min::test(int value){
  return value_ > value;
}

/* informe que la classe est Min */
bool BobbyMinMax::Min::isMax(){
  return false;
}

/* constructeur de Max */
BobbyMinMax::Max::Max(Node* parent, int cell, int grid) : Node(parent,INT_MIN,cell,grid){
}

/* test utiliser par Max */
bool BobbyMinMax::Max::test(int value){
  return value_ < value;
}

/* informe que la classe est Max */
bool BobbyMinMax::Max::isMax(){
  return true;
}

/* Algorithme de choix de coup minmax */
void BobbyMinMax::algorithm(int& grid, int& cell) {
  std::vector<std::thread> listThread;
  job_ = true;
  Board tmp = board_;
  Max origin(nullptr,0,0);
  createNode(tmp,depth_,&origin);
  for (int i = 0; i < nbThread_; i++) {
    listThread.emplace_back(&BobbyMinMax::funcThread,this);
  }
  while (origin.getNbChild() != 0) {
  }
  job_ = false;
  for (size_t i = 0; i < listThread.size(); i++) {
    listThread[i].join();
  }
  grid = origin.getGrid();
  cell = origin.getCell();
}

/* Fonction des threads */
void BobbyMinMax::funcThread() {
  std::function<void()> task;

  /* on attend la fin des taches a faire */
  while (job_) {
    taskMutex_.lock();
    /* on prend et on execute une tache a faire si on peut */
    if (!taskQueue_.empty()) {
      task = taskQueue_.front();
      taskQueue_.pop();
      taskMutex_.unlock();
      task();
    }
    else{
      taskMutex_.unlock();
    }
  }
}

/* Fonction de creation de Node destiner a la queue de tache a faire */
void BobbyMinMax::createNode(const Board& board, int depth, Node* parent){
  std::vector<std::pair<int,int>> moves;
  Board tempBoard = board;
  int newDepth = depth - 1;

  possibleMove(tempBoard,moves);
  parent->setNbChild(moves.size());

  for (size_t i = 0; i < moves.size(); i++) {
    std::pair<int,int> move = moves[i];
    tempBoard = board;
    if (parent->isMax()) {
      tempBoard.update(symbole_,move.first,move.second);
    }else{
      tempBoard.update(opponent_,move.first,move.second);
    }
    if (newDepth == 0 || tempBoard.gameState() != NOTHING) {
      parent->updateMe(heuristic(tempBoard),move.first,move.second);
    }else{
      taskMutex_.lock();
      /* on push les creations de noeuds pour qu'elle soit executer par d'autre threads */
      taskQueue_.push([this,tempBoard,newDepth,parent,move] {
        Node* node;
        if (parent->isMax()) {
          node = new Min(parent,move.first,move.second);
        }else{
          node = new Max(parent,move.first,move.second);
        }
        this->createNode(tempBoard,newDepth,node);
      });
      taskMutex_.unlock();
    }
  }
}

/* Heuristique de l'algorithme */
int BobbyMinMax::heuristic(Board& board){
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
int BobbyMinMax::evaluateLine(int line){
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
int BobbyMinMax::caseValue(symbole cell){
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
void BobbyMinMax::possibleMove(Board& board, std::vector<std::pair<int,int>>& move){
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
