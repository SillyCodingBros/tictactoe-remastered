#include "MinMax.hpp"

std::mutex print_;

// Constructeur de MinMax
MinMax::MinMax(symbole signe, symbole opponent, Board& board, int nbThread, int depth)
              :   IComputer(signe, board),
                  game_(true),
                  opponent_(opponent),
                  nbThread_(nbThread),
                  depth_(depth)
{
  // Lancement des threads
  std::cerr << end_ << " Création de " << nbThread_ << " threads" << '\n';
  for (auto i = 0; i < nbThread_; i++) {
    threads_.emplace_back(&MinMax::funcThread, this);
    std::cerr << "ID : " << threads_[i].get_id() << '\n';
  }
  std::cerr << "threads_.size() : " << threads_.size() << '\n';

}

// Retourne le symbole de MinMax
symbole MinMax::getSymbole(){
  return symbole_;
}

// Ajoute une tâche à la liste
void MinMax::pushTask(std::function<void()> task, bool upORdown){
  taskMutex_.lock();

  if(upORdown){
    taskQueue_.push_front(task);
  }
  else{
    taskQueue_.push_back(task);
  }

  taskMutex_.unlock();
}

// Réalise une tâche de la queue
void MinMax::completeATask(){
  assert(taskQueue_.size() > 0);

  auto task = taskQueue_.front();

  taskQueue_.pop_front();
  taskMutex_.unlock();

  task();
}


//Retourne la liste des coups possibles pour une grille donnée
void MinMax::possibleMoves(Board& board, std::vector<std::pair<int,int>>& moves){
  int curGrid = board.getCurGrid();

  if (curGrid == -1) {
    for(auto i = 0; i < 81; ++i) {
      if (board.getCell(i) == NOTHING) {
        moves.emplace_back((i / 9), (i % 9));
      }
    }
  }
  else {
    for (auto i = 0; i < 9; ++i){
      if (board.getCell((curGrid * 9) + i) == NOTHING){
        moves.emplace_back(curGrid, i);
      }
    }
  }
}

// Fonction des threads / Réalise une tâche si il y en a en attente
void MinMax::funcThread(){
  while (game_) {

    taskMutex_.lock();

    if(end_ && !taskQueue_.empty()){
        taskQueue_.clear();
    }

    if (!taskQueue_.empty()) {
      completeATask();
    }
    else{
      taskMutex_.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }
}

// Écrit les coordonnées du coup choisit dans les arguments
void MinMax::algorithm(int& grid, int& cell){

  Board tmpBoard = board_;
  int depth = depth_;

  end_ = false;

  // Ajout du noeud origine
  origin_ = new Node(INFINITE_MIN);
  createChildren(origin_, tmpBoard, depth);

  // Attente du résultat
  while (origin_->nbChild_ > 0);
  end_ = true;

  // Place les coordonnées du coup choisit
  grid = origin_->grid_;
  cell = origin_->cell_;

}

//Calcul de l'heuristique d'un plateau
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
    return 5000;
  }
  else if (line == -6){
    return -5000;
  }
  else if (line == -3){
    return 5000/2;
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

// Met à jour ou non sa valeur de son parent
void MinMax::Node::update(int value, int grid, int cell){

  parent_->nodeMutex_.lock();

  bool change = false;
  bool win = false;
  bool go = false;

  if (parent_->nbChild_ > 0) {

    go = true;

    if (parent_->max_) {
      if (value > parent_->value_) {
        change = true;
      }
      if (value >= 5000) {
        win = true;
      }
    }
    else{
      if (value < parent_->value_) {
        change = true;
      }
      if (value <= -5000) {
        win = true;
      }
    }

    if (!win) {
      parent_->nbChild_ -= 1;
    }
    else{
      parent_->nbChild_ = 0;
    }
  }

  if (change) {

    parent_->value_ = value;

    if (parent_->parent_ == nullptr) {

      parent_->grid_ = grid;
      parent_->cell_ = cell;
    }
  }

  if ((parent_->nbChild_ == 0) && go) {
    if (parent_->parent_ != nullptr) {

      parent_->update(parent_->value_, parent_->grid_, parent_->cell_);
    }
  }
  parent_->nodeMutex_.unlock();
}

// Création de l'arbre
void MinMax::createChildren(Node* parent, const Board& board, int depth){
  Board tmpBoard = board;
  if (depth && tmpBoard.gameState() == NOTHING) {

    int value;
    symbole player;

    if(parent->max_){
      value = INFINITE_MAX;
      player = getSymbole();
    }
    else{
      value = INFINITE_MIN;
      player = opponent_;
    }

    std::vector<std::pair<int,int>> moves;

    possibleMoves(tmpBoard, moves);
    parent->nbChild_ = moves.size();
    int tmpMaxNbChild = parent->nbChild_;

    for (int i = 0; i < tmpMaxNbChild; ++i){
      Board nextMoveBoard = tmpBoard;

      int nextGrid = moves[i].first;
      int nextCell = moves[i].second;

      nextMoveBoard.update(player, nextGrid, nextCell);

      //Push création des Nodes enfant
      bool upORdown = false;
      if(!(depth-1) || nextMoveBoard.gameState() != NOTHING){
        upORdown = true;
      }

      pushTask([=] {
          Node* child;

          child = new Node(parent, value, !parent->max_, nextGrid, nextCell);

          createChildren(child, nextMoveBoard, depth-1);
      }, upORdown);

    }
  }
  else{
    parent->nbChild_ = 0;
    parent->update(heuristic(tmpBoard), parent->grid_, parent->cell_);
  }
}


// Constructeur de Node
MinMax::Node::Node(Node* parent, int value, bool max, int grid, int cell)
              : parent_(parent),
                value_(value),
                max_(max),
                grid_(grid),
                cell_(cell)
{

}

// Constructeur pour origin
MinMax::Node::Node(int min)
              : parent_(nullptr),
                value_(min),
                nbChild_(1),
                max_(true),
                grid_(-1),
                cell_(-1)
{

}