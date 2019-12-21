#include "MinMax.hpp"

// Création d'un objet MinMax
MinMax::MinMax(symbole signe, symbole opponent, Board* board, int nbThread, int depth)
              : IComputer(signe, board), opponent_(opponent), depth_(depth), nbThread_(nbThread){
}

/* constructeur de Node */
MinMax::Node::Node(Node* parent, int value, int grid, int cell) : parent_(parent), value_(value), grid_(grid), cell_(cell){
}

/* Fonction pour update le node avec une nouvelle valeur */
void MinMax::Node::updateMe(int value, int grid, int cell){
  Node* nextUpdate = this;
  nodeMutex_.lock();
  //std::cout << "node in traitment : " << this << '\n';
  if (chemin()) {
    if (test(value)) {
      value_ = value;
      if (parent_ == nullptr) {
        grid_ = grid;
        cell_ = cell;
        //std::cout << "origin " << this << " vaut : " << grid_ << ", " << cell_ << " when nbChild = " << nbChild_ << '\n';
      }
    }
    nbChild_ -= 1;
    //std::cout << "node " << this << " has value = " << value_ << " with nbChild = " << nbChild_ << '\n';
    nextUpdate = alphaBeta();
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
MinMax::Node* MinMax::Node::alphaBeta(){
  Node* result = nullptr;
  if (parent_ != nullptr) {
    result = this;
    parent_->nodeMutex_.lock();
    if (parent_->parent_ != nullptr) {
      parent_->parent_->nodeMutex_.lock();
      if (!parent_->parent_->test(value_)) {
        parent_->parent_->nbChild_ -=1;
        parent_->nbChild_ = 0;
        //std::cout << "elaguage success" << '\n';
        result = parent_->parent_;
      }
      parent_->parent_->nodeMutex_.unlock();
    }
    parent_->nodeMutex_.unlock();
  }
  return result;
}

/* fonction qui test si la branche actuelle est élagué */
bool MinMax::Node::chemin(){
  Node* tmp = parent_;
  while (tmp != nullptr) {
    if (tmp->nbChild_ == 0) {
      //std::cout << "branche elagué" << '\n';
      return false;
    }
    tmp = tmp->parent_;
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
MinMax::Min::Min(Node* parent, int grid, int cell) : Node(parent,INT_MAX,grid,cell){
}

/* test utiliser par Min */
bool MinMax::Min::test(int value){
  return value_ > value;
}

/* informe que la classe est Min */
bool MinMax::Min::isMax(){
  return false;
}

/* constructeur de Max */
MinMax::Max::Max(Node* parent, int cell, int grid) : Node(parent,INT_MIN,cell,grid){
}

/* test utiliser par Max */
bool MinMax::Max::test(int value){
  return value_ < value;
}

/* informe que la classe est Max */
bool MinMax::Max::isMax(){
  return true;
}

// Algorithme de choix de coup minmax
void MinMax::algorithm(int& grid, int& cell) {
  std::vector<std::thread> listThread;
  job_ = true;
  Board tmp = *board_;
  Max origin(nullptr,0,0);
  createNode(tmp,depth_,&origin);
  for (int i = 0; i < nbThread_; i++) {
    listThread.emplace_back(&MinMax::funcThread,this);
  }
  //std::cerr << "nbThread" << listThread.size() << '\n';
  while (origin.getNbChild() != 0) {
/*
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cerr << "child origin : " << origin.getNbChild() << '\n';
    std::cerr << "list thread" << listThread.size() << '\n';
*/
  }
  //std::cerr << "job done" << '\n';
  job_ = false;
  //std::cerr << "list thread" << listThread.size() << '\n';
  for (size_t i = 0; i < listThread.size(); i++) {
    listThread[i].join();
  }
  grid = origin.getGrid();
  cell = origin.getCell();
}

/* Fonction des threads */
void MinMax::funcThread() {
  std::function<void()> task;

  while (job_) {
    taskMutex_.lock();
    if (!taskQueue_.empty()) {
      task = taskQueue_.front();
      taskQueue_.pop();
      taskMutex_.unlock();
      task();
    }
    else{
      //std::cout << "wait taskQueue or job_" << '\n';
      taskMutex_.unlock();
    }
    //task();
  }
}

/* Fonction de creation de Node destiner a la queue de tache a faire */
void MinMax::createNode(const Board& board, int depth, Node* parent){
/*
  Board debug = board;
  std::cout << "==============================================" << '\n';
  debug.draw();
  std::cout << "node :" << parent << '\n';
  std::cout << "\t-depth : " << depth << '\n';
  std::cout << "\t-value : " << parent->getValue() << '\n';
  std::cout << "\t-cell : " << parent->getCell() << '\n';
  std::cout << "\t-grid : " << parent->getGrid() << '\n';
*/
  std::vector<std::pair<int,int>> moves;
  Board tempBoard = board;
  int newDepth = depth - 1;
  possibleMove(tempBoard,moves);
  parent->setNbChild(moves.size());
/*
  std::cout << "\t-nbChild : " << parent->getNbChild() << '\n';
  std::cout << "==============================================" << '\n';
*/
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
      taskQueue_.push([this,tempBoard,newDepth,parent,move] {
        Node* node;
        if (parent->isMax()) {
          node = new Min(parent,move.first,move.second);
        }else{
          node = new Max(parent,move.first,move.second);
        }
/*
        std::cout << "create new node : " << node << " with parent : " << parent << '\n';
        std::cout << "task queue size = " << taskQueue_.size() << '\n';

        if (newDepth <= 1) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
*/
        this->createNode(tempBoard,newDepth,node);
      });
      taskMutex_.unlock();
    }
  }
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
