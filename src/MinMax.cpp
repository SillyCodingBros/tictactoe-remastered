#include "MinMax.hpp"

// Création d'un objet MinMax
MinMax::MinMax(symbole signe,
              symbole opponent,
              Board* board,
              int nbThread,
              int depth)
              : IComputer(signe, board),
                opponent_(opponent),
                game_(true),
                depth_(depth){
  //creation des threads
  for (auto i = 0; i < nbThread; i++) {
    listThread_.emplace_back(&MinMax::funcThread, this);
  }
}


// Constructeur vide de task
MinMax::task::task(){
}


// Constructeur de task
MinMax::task::task(Board& board, node* curNode, int depth, bool up)
                : board_(board), curNode_(curNode), depth_(depth), up_(up){
}


// Constructeur vide de node
MinMax::node::node(){
}


// Constructeur de nouveau node
MinMax::node::node(int grid, int cell, node* parent, int max, int min)
                : grid_(grid), cell_(cell), parent_(parent){
  if (parent_->max_) {
    max_ = false;
    value_ = min;
  }
  else{
    max_ = true;
    value_ = max;
  }
}


// Retourne la grille d'un noeud
int MinMax::node::getGrid(){
  return grid_;
}


// Retourne la cellule d'un noeud
int MinMax::node::getCell(){
  return cell_;
}


// Retourne le nombre d'enfant en attente de traitement
int MinMax::node::getWorkChild(){
  return workChild_;
}


// Modifie le nombre d'enfant restant à traiter du noeud
void MinMax::node::setWorkChild(int workChild){
  workChild_ = workChild;
}


// Retourne le noeud parent d'un noeud enfant
MinMax::node* MinMax::node::getParent(){
  return parent_;
}


// Fonction pour charger les attributs du noeud originel
void MinMax::node::loadOrigin(int minValue){
  value_ = minValue;
  max_ = true;
  parent_ = nullptr;
}


// Retourne si le noeud est min ou max
bool MinMax::node::getMax(){
  return max_;
}


// Retourne le nombre d'enfant en attente de traitement du parent
int MinMax::node::getParentWorkChild(){
  return parent_->workChild_;
}


// Gère le passage de l'heuristique au parent selon minmax
void MinMax::node::submitWork(){
  bool mustSubmit = false;

  parent_->nodeMutex_.lock();

  if (parent_->max_) {
    if (value_ > parent_->value_) {
      mustSubmit = true;
    }
  }
  else{
    if (value_ < parent_->value_) {
      mustSubmit = true;
    }
  }

  if (mustSubmit) {
    parent_->value_ = value_;
    if (parent_->isOrigin()) {
      parent_->grid_ = grid_;
      parent_->cell_ = cell_;
    }
  }

  std::cerr << "\nparent->workChild = " << parent_->workChild_ << '\n';

  parent_->workChild_ -= 1;

  std::cerr << "decr parent->workChild = " << parent_->workChild_ << '\n';
  std::cerr << "parent->grid = " << parent_->grid_ << '\n';
  std::cerr << "parent->cell = " << parent_->cell_ << '\n';
  std::cerr << "parent->value = " << parent_->value_ << "\n\n";

  parent_->nodeMutex_.unlock();
}


// Modifie la valueur du noeud
void MinMax::node::setValue(int value){
  value_ = value;
}


// Détermine si le noeud est origine
bool MinMax::node::isOrigin(){
  if (parent_ == nullptr) {
    return true;
  }
  return false;
}


// Algorithme de choix de coup minmax
void MinMax::algorithm(int& grid, int& cell) {
  node origin;
  Board tmp = *board_;

  origin.loadOrigin(minValue_);
  emplaceTask(tmp, &origin, depth_, false);
  // Attente de la fin des tâches
  while (true) {

/*
    print.lock();
    std::cerr << "stop : " << origin.getWorkChild() << " " << taskQueue_.empty() << '\n';
    print.unlock();
*/

    if (origin.getWorkChild() == 0 && taskQueue_.empty()) {
      break;
    }
  }
  grid = origin.getGrid();
  cell = origin.getCell();

  game_ = false;
}


// Ajoute une tâche à la fin de la queue
void MinMax::pushTask(task& task){
  taskMutex_.lock();
  taskQueue_.push(task);
  taskMutex_.unlock();
}


// Construit une tâche au début de la queue
void MinMax::emplaceTask(Board& board, node* curNode, int depth, bool up){
  taskMutex_.lock();
  taskQueue_.emplace(board, curNode, depth, up);
  taskMutex_.unlock();
}

// Gestion des noeuds soumis à un parent
void MinMax::handleSubmitedWork(task& task){
  node* node = task.curNode_;
  if (!node->isOrigin()) {

    print.lock();

    node->submitWork();

    print.unlock();

    if (node->getParentWorkChild() == 0) {
      task.curNode_ = task.curNode_->getParent();
      pushTask(task);
    }
  }
}


//Gère la construction de l'arbre (sapin) pour un noeud terminal
void MinMax::handleTerminalNode(task& task){
  task.curNode_->setValue(heuristic(task.board_));
  emplaceTask(task.board_, task.curNode_, 0, true);
}


//Gère la construction de l'arbre (sapin) pour un noeud
void MinMax::handleNode(task& task){
  std::vector<std::pair<int,int>> moves;

  possibleMove(task.board_, moves);
  task.curNode_->setWorkChild(moves.size());

  print.lock();
  std::cerr << "\npossible moves number : " << moves.size() << '\n';
  print.unlock();

  for (size_t i = 0; i < moves.size(); ++i) {
    Board tmpBoard = task.board_;

    if (task.curNode_->getMax()) {
      tmpBoard.update(symbole_, moves[i].first, moves[i].second);
    }
    else{
      tmpBoard.update(opponent_, moves[i].first, moves[i].second);
    }

    print.lock();
    std::cerr << "grid " << moves[i].first << '\n';
    std::cerr << "cell " << moves[i].second << '\n';
    print.unlock();

    emplaceTask(tmpBoard,
                new node(moves[i].first,
                        moves[i].second,
                        task.curNode_,
                        maxValue_,
                        minValue_),
                task.depth_-1,
                false);
  }
}


// Gestion des threads
void MinMax::funcThread(){
  task task;
  while (game_) {
    while (!taskQueue_.empty()) {

      print.lock();
      std::cerr << "queue size = " << taskQueue_.size() << '\n';
      print.unlock();

      taskMutex_.lock();
      task = taskQueue_.front();
      taskQueue_.pop();
      taskMutex_.unlock();

      if (task.up_) {
        handleSubmitedWork(task);
      }
      else{
        if (task.depth_ == 0 || task.board_.gameState() != NOTHING) {
          handleTerminalNode(task);
        }
        else{
          handleNode(task);
        }
      }
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
  //printf("line = %d\n", line);
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
    return 3;
  }
  else if (line < 0){
    return line/2;
  }
  return line;
}


// Associe une valeur à un symbole
int MinMax::caseValue(symbole cell){
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
        print.lock();
        std::cerr << "case" << curGrid << "," << i << "ok" << '\n';
        print.unlock();
        move.emplace_back(curGrid, i);
      }
    }
  }
}
