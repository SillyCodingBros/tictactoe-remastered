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
  // Création des threads
  std::cerr << "Création de " << nbThread << " threads" << '\n';
  for (auto i = 0; i < nbThread; i++) {
    listThread_.emplace_back(&MinMax::funcThread, this);
    std::cerr << "ID : " << listThread_[i].get_id() << '\n';

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
    value_ = max;
  }
  else{
    max_ = true;
    value_ = min;
  }
}


// Retourne la grille d'un noeud
int MinMax::node::getGrid(){
  return grid_;
}

// Modifie le valeur de la grille d'un noeud
void MinMax::node::setGrid(int grid){
  grid_ = grid;
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
  workChild_ = 1;
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
  bool bestMove = false;

  parent_->nodeMutex_.lock();

  std::cerr << "\n\nparent max : " << parent_->max_
            << " value_ : " << value_
            << " parent_->value_ : " << parent_->value_ << '\n';

  if (parent_->max_) {
    if (value_ == INT_MAX){
      bestMove = true;
    }
    if (value_ > parent_->value_) {
      std::cerr << "\n\nC'est plus grand que le parent : " << value_ << '\n';
      mustSubmit = true;
    }
  }
  else{
    if (value_ == INT_MIN) {
      bestMove = true;
    }
    if (value_ < parent_->value_) {
      std::cerr << "\n\nC'est plus petit que le parent : " << value_ << '\n';
      mustSubmit = true;
    }
  }

  if (mustSubmit) {
    std::cerr << "Je remplace" << '\n';
    parent_->value_ = value_;
    std::cerr << "value : " << value_ << " parent_->value_ : " << parent_->value_<< '\n';
    if (parent_->isOrigin()) {
      std::cerr << "Replace la position" << '\n';
      parent_->grid_ = grid_;
      parent_->cell_ = cell_;
      std::cerr << "grid : " << grid_ << " parent_->grid_ : " << parent_->grid_
                << " cell : " << cell_<< " parent_->cell_ : " << parent_->cell_ << '\n';
    }
  }

  if (bestMove) {
    parent_->workChild_ = 0;
  }
  else{
    std::cerr << "\nparent->workChild = " << parent_->workChild_ << '\n';

    parent_->workChild_ -= 1;
  }

  if (parent_->isOrigin()) {
    std::cerr << "PARENT IS ORIGIN!" << '\n';
    if (parent_->workChild_ == 0) {
      std::cerr << "END IS NEAR / NO MORE WORKING CHILD" << '\n';
    }
  }

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

// Retourne la valueur du noeud
int MinMax::node::getValue(){
  return value_;
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

  print.lock();
  std::cerr << "Nouveau coup de l'IA" << '\n';
  print.unlock();

  origin.loadOrigin(minValue_);
  origin_ = &origin;
  emplaceTask(tmp, &origin, depth_, false);
  end_ = false;
  // Attente de la fin des tâches
  while (!end_) {

/*
    print.lock();
    std::cerr << "stop : " << origin.getWorkChild() << " " << taskQueue_.empty() << '\n';
    print.unlock();
*/

    if (
        origin.getWorkChild() == 0
        //&&
        //taskQueue_.empty()
      ) {

      //print.lock();
      std::cerr << "END OF THINGS!" << '\n';
      end_ = true;
      taskMutex_.lock();
      if (!taskQueue_.empty()) {
        std::cerr << "JE VIDE LES MEUBLES DANS ALGO!" << '\n';
        //for (size_t i = 0; i < taskQueue_.size(); ++i) {
          taskQueue_.clear();
        //}
        origin_->setWorkChild(0);
      }
      taskMutex_.unlock();
      /*for (size_t i = 0; i < listThread_.size(); i++) {
        listThread_[i].join();
      }

      std::cerr << "END OF JOIN!" << '\n';*/
      //print.unlock();

      //break;
    }
  }
  grid = origin.getGrid();
  cell = origin.getCell();

  //game_ = false;
}


// Ajoute une tâche à la fin de la queue
void MinMax::pushTask(task& task){

    taskMutex_.lock();

    if (origin_->getWorkChild() != 0 && !end_) {

      //taskMutex_.lock();

      print.lock();
      std::thread::id this_id = std::this_thread::get_id();
      std::cerr << "NEW PUSH FROM "<< this_id << " :\n"
      << "\ttask->curNode->grid = " << task.curNode_->getGrid() << '\n'
      << "\ttask->curNode->cell = " << task.curNode_->getCell() << '\n'
      << "\ttask->curNode->value = " << task.curNode_->getValue() << '\n'
      << "\ttask.depth_ = " << task.depth_ << '\n'
      << "\ttask.up_ = " << task.up_ << "\n\n";
      print.unlock();

      if (task.curNode_->getWorkChild() == 0) {
        taskQueue_.push_front(task);
      }
      else{
        taskQueue_.push_back(task);
      }
      //taskMutex_.unlock();

    }

    taskMutex_.unlock();



}


// Construit une tâche au début de la queue
void MinMax::emplaceTask(Board& board, node* curNode, int depth, bool up){

  //if (!taskQueue_.empty()) {
    taskMutex_.lock();
  //}

  if (curNode->isOrigin()) {
    std::cerr << "J'EMPLACE ORIGIN" << '\n';
    //origin_ = curNode;
  }



    //if (!taskQueue_.empty() ||
          //(curNode->isOrigin() || curNode->getParent()->isOrigin())) {

      print.lock();
      std::thread::id this_id = std::this_thread::get_id();
      ////std::cerr << "NEW EMPLACE FROM "<< this_id << ":\n"
      ////<< "\ttask->curNode->grid = " << curNode->getGrid() << '\n'
      ////<< "\ttask->curNode->cell = " << curNode->getCell() << '\n'
      ////<< "\ttask->curNode->value = " << curNode->getValue() << '\n'
      ////<< "\tdepth = " << depth << '\n'
      ////<< "\tup = " << up << "\n\n";
      //print.unlock();
      //if (up) {
        //taskQueue_.emplace_back(board, curNode, depth, up);
      //}
      //else{
        taskQueue_.emplace_front(board, curNode, depth, up);
      //}
      //print.lock();
      ////std::cerr << "J'ai BIEN EMPLACE" << '\n';
      print.unlock();

      /*if(taskMutex_.try_lock()){
        taskMutex_.unlock();
      }
      else{*/
        taskMutex_.unlock();
      //}

    //}

    //taskMutex_.unlock();

}

// Gestion des noeuds soumis à un parent
void MinMax::handleSubmitedWork(task& task){
  node* node = task.curNode_;
  if (!node->isOrigin() && node->getParentWorkChild() > 0) {

    if (node->getWorkChild() == 0) {

      print.lock();

      node->submitWork();

      print.unlock();

      if (
          //node->getParentWorkChild() == 0
          //&&
          !node->getParent()->isOrigin()
        ) {
        print.lock();
        std::cerr << "\nJE PUSH POUR FAIRE REMONTER AU TOP" << '\n';
        print.unlock();

        task.curNode_ = task.curNode_->getParent();
        pushTask(task);

      }

    }
    else{
      print.lock();
      std::cerr << "\nJE PUSH POUR FAIRE REMONTER AU BACK" << '\n';
      print.unlock();
      pushTask(task);
    }
  }
}


//Gère la construction de l'arbre (sapin) pour un noeud terminal
void MinMax::handleTerminalNode(task& task){
  task.curNode_->setValue(heuristic(task.board_));
  print.lock();
  ////std::cerr << "\nheuristic : " << heuristic(task.board_) << '\n';
  ////std::cerr << "curNode->value : " << task.curNode_->getValue() << '\n';
  print.unlock();
  emplaceTask(task.board_, task.curNode_, 0, true);
}


//Gère la construction de l'arbre (sapin) pour un noeud
void MinMax::handleNode(task& task){
  std::vector<std::pair<int,int>> moves;

  if (task.curNode_->isOrigin()) {
    task.curNode_->setGrid(task.board_.getCurGrid());
  }

  possibleMove(task.board_, moves);
  task.curNode_->setWorkChild(moves.size());

  print.lock();
  ////std::cerr << "\npossible moves number : " << moves.size() << '\n';
  print.unlock();

  for (size_t i = 0; i < moves.size(); ++i) {
    Board tmpBoard = task.board_;

    print.lock();
    ////std::cerr << "\nPB THREAD " << std::this_thread::get_id() << '\n';
    ////std::cerr << "PB AVEC UPDATE ?" << '\n';

    if (task.curNode_->getMax()) {
      tmpBoard.update(symbole_, moves[i].first, moves[i].second);
    }
    else{
      tmpBoard.update(opponent_, moves[i].first, moves[i].second);
    }

    ////std::cerr << "PAS DE PB AVEC UPDATE !" << '\n';
    print.unlock();


    /*print.lock();
    std::cerr << "grid " << moves[i].first << '\n';
    std::cerr << "cell " << moves[i].second << '\n';
    print.unlock();*/

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

    //taskMutex_.lock();
    while (!taskQueue_.empty()) {
    //(origin_->getWorkChild() != 0) {
      print.lock();
      ////std::cerr << "\nJe bloque sur le MUTEX ?"
      ////          << " size : " << taskQueue_.size()
      ////          << " originWorkChild : " << origin_->getWorkChild()
      ////          << " end : " << end_ << '\n';
      print.unlock();
      taskMutex_.lock();

      if (end_){
        std::cerr << "JE VIDE LES MEUBLES !" << '\n';
        //for (size_t i = 0; i < taskQueue_.size(); ++i) {
          taskQueue_.clear();
        //}
        origin_->setWorkChild(0);
      }

      if (!taskQueue_.empty()
          &&
          (origin_->getWorkChild() > 0)
          &&
          !end_){

        print.lock();
        ////std::cerr << "\nIAM THREAD " << std::this_thread::get_id() << '\n';
        std::cout <<  std::this_thread::get_id() << '\n';
        ////std::cerr << "queue size = " << taskQueue_.size() << "\n\n";
        std::cout << " queue size = " << taskQueue_.size() << "\n\n";
        //print.unlock();

        //print.lock();
        std::cout << "ORIGIN WORKCHILD : " << origin_->getWorkChild() << '\n';
        ////std::cerr << "ORIGIN WORKCHILD : " << origin_->getWorkChild() << '\n';
        ////std::cerr << "Je suis RENTRÉ !" << std::this_thread::get_id() << '\n';
        //taskMutex_.lock();
        task = taskQueue_.front();
        taskQueue_.pop_front();

        ////std::cerr << "J'ai PRIS UNE TÂCHE !" << '\n';
        print.unlock();

        taskMutex_.unlock();

        if (task.up_) {
          handleSubmitedWork(task);
          //taskMutex_.unlock();
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
      else{
        taskMutex_.unlock();
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //print.lock();
    //std::cerr << "FIN DE BOUCLE " << std::this_thread::get_id() << '\n';
    //print.unlock();
    //taskMutex_.unlock();
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
    return maxValue_/2;
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
        /*print.lock();
        std::cerr << "case" << curGrid << "," << i << "ok" << '\n';
        print.unlock();*/
        move.emplace_back(curGrid, i);
      }
    }
  }
}
