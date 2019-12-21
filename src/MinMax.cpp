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

  std::cerr << end_ << " Création de " << nbThread_ << " threads" << '\n';
  for (auto i = 0; i < nbThread_; i++) {
    //auto ptr = new FunctionForThread(this);
    threads_.emplace_back(&MinMax::funcThread, this);
    std::cerr << "ID : " << threads_[i].get_id() << '\n';
  }
  std::cerr << "threads_.size() : " << threads_.size() << '\n';

}

// Constructeur de FunctionForThread
/*FunctionForThread::FunctionForThread(MinMax* minmax):minmax_(minmax)
{

}
*/

// Retourne le symbole de MinMax
symbole MinMax::getSymbole(){
  return symbole_;
}

// Ajoute une tâche à la queue
void MinMax::pushTask(std::function<void()> task, bool upORdown){
  taskMutex_.lock();

  if(upORdown){
    taskQueue_.push_front(task);
  }
  else{
    taskQueue_.push_back(task);
  }

  //taskQueue_.push(task);

  taskMutex_.unlock();
}

// Réalise une tâche de la queue
void MinMax::completeATask(){
  //taskMutex_.lock();
  //if (!taskQueue_.empty()) {
    assert(taskQueue_.size() > 0);

     /*

    print_.lock();
    std::cout << "queue size : " << taskQueue_.size() << '\n';
    std::cout << "origin_->nbChild_ : " << origin_->nbChild_ << '\n';
    //countMutex_.lock();
    //std::cout << "Threads en travail : " << count_ << '\n';
    //countMutex_.unlock();
    //std::cerr << "\n\nqueue size : " << taskQueue_.size() << '\n';
    //std::cerr << std::this_thread::get_id() << ": Je prend une tâche !" << '\n';
    print_.unlock();
     */

    auto task = taskQueue_.front();
    //taskQueue_.pop();
    taskQueue_.pop_front();
    taskMutex_.unlock();

    /*

    print_.lock();
    std::cerr << "J'ai pris un tâche" << '\n';
    std::cerr << "queue size : " << taskQueue_.size() << '\n';
    print_.unlock();

    */

    task();

    /*

    print_.lock();
    std::cerr << std::this_thread::get_id() << ": J'ai fais la tâche !" << '\n';
    std::cerr << "queue size : " << taskQueue_.size() << "\n\n";
    print_.unlock();

    */

  //}
  //else{
  //  taskMutex_.unlock();
  //}
}

/*
  /!\ Fonction PARFAITE /!\
  Retourne la liste des coups possibles pour une grille donnée
*/
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
  /*
  print_.lock();
  //board.draw();
  std::cerr << "POSSIBLE MOVES: Dans la gille : " << curGrid+1 << " il y a " << moves.size() << " mouvements possibles"<< '\n';
  print_.unlock();
  */
}

// Gère les threads
void MinMax::funcThread(){
  while (game_) {
    /*
    countMutex_.lock();
    count_++;
    countMutex_.unlock();
    */
    taskMutex_.lock();

    if(end_ && !taskQueue_.empty()){
      //for (size_t i = 0; i < taskQueue_.size(); i++) {
        std::cerr << "CLEAR !!!" << '\n';
        //taskQueue_.pop();
        taskQueue_.clear();
      //}
    }

    if (!taskQueue_.empty()) {
      completeATask();
    }
    else{
      taskMutex_.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    /*
    countMutex_.lock();
    count_--;
    countMutex_.unlock();
    */
  }
    /*
    //std::cout << "Je bloque origin" << '\n';
    origin_->nodeMutex_.lock();
    if(origin_->nbChild_ == 0){
      std::cout << "C'est la fin!" << '\n';
      end_ = true;
    }
    origin_->nodeMutex_.unlock();
  }
  std::cout << "funcThread out!" << '\n';
  */

}

// Écrit les coordonnées du coup choisit dans les arguments
void MinMax::algorithm(int& grid, int& cell){

  //if(!end_){

  Board tmpBoard = board_;
  int depth = depth_;

  /*

  print_.lock();
  std::cout << "\nNouveau coup de l'IA" << '\n';
  std::cerr << "\nNouveau coup de l'IA" << '\n';
  print_.unlock();

  */

  //endMutex_.lock();
  end_ = false;
  //endMutex_.unlock();

  // Ajout du noeud origine
  origin_ = new Node(INFINITE_MIN);
  //origin_->nodeMutex_.lock();
  //origin_->nbChild_ = 1;
  //origin_->nodeMutex_.unlock();
  createChildren(origin_, tmpBoard, depth);

  // Création des threads
  //handleThreads();

  while (origin_->nbChild_ > 0);
  end_ = true;




  //std::cout << "ICI !" << '\n';

  //std::cout << origin_->grid_ << ":" << origin_->cell_ << '\n';

  //assert(2 == 3);

  //assert(tmpBoard.update(symbole_, origin_->grid_, origin_->cell_));

  grid = origin_->grid_;
  cell = origin_->cell_;

//}

}

//Calcul de l'heuristique
int MinMax::heuristic(Board& board){
  int sum = 0;
  for (auto i=0; i < 9; ++i){
    for (auto j=0; j < 8; ++j){
      sum += evaluateLine(caseValue(board.getCell(POSSIBILITIES_[0 + (j * 3)] + (9 * i))) +
                           caseValue(board.getCell(POSSIBILITIES_[1 + (j * 3)] + (9 * i))) +
                           caseValue(board.getCell(POSSIBILITIES_[2 + (j * 3)] + (9 * i))));
    }
  }
  //push le travail
  /*
  print_.lock();
  std::cerr << "Je push l'heuristique de : " << sum << '\n';
  print_.unlock();
  */

  //pushTask([this, sum]
  //          { update(sum); });
  return sum;
  //update();
}

// Retourne le poids associé à une ligne*
//*(Somme des valeurs associées aux symboles qui la compose)
int MinMax::evaluateLine(int line){
  //printf("line = %d\n", line);
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
// Retourne la grille d'un noeud
int MinMax::Node::getGrid(){
  return grid_;
}

// Retourne la grille d'un noeud
int MinMax::Node::getCell(){
  return cell_;
}
*/

// Met à jour ou non sa valeur
void MinMax::Node::update(int value, int grid, int cell){
  /*
  print_.lock();
  std::cerr << "Je dois essayer d'UPDATE mon parent" <<
              "\nparent : " << parent_ <<
              "\nme : " << this << '\n';
  std::cerr << "nbChild_ : " << nbChild_ << '\n';
  std::cerr << "parent_->nbChild_ : " << parent_->nbChild_ << '\n';
  std::cerr << grid_+1 << ":" << cell_+1 << '\n';
  std::cerr << parent_->grid_+1 << ":" << parent_->cell_+1 << '\n';
  print_.unlock();
  */

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
      //std::cout << "WIN FOUND !" << '\n';
      parent_->nbChild_ = 0;
    }
  }

  if (change) {
    /*
    print_.lock();
    std::cerr << "Je dois remplacer ma VALUE de mon PARENT" << '\n';
    print_.unlock();
    */

    parent_->value_ = value;

    if (parent_->parent_ == nullptr) {
      /*
      //print_.lock();
      std::cout << "ORIGIN CHILD: Je dois UPDATE ses coordonnés" << '\n';
      //print_.unlock();
      */

      parent_->grid_ = grid;
      parent_->cell_ = cell;
    }
  }

  //parent_->nodeMutex_.unlock();

  if ((parent_->nbChild_ == 0) && go) {
    //push le travail
    if (parent_->parent_ != nullptr) {
      /*
      print_.lock();
      std::cerr << "Mon PARENT doit UPDATE mon GPARENT" << '\n';
      print_.unlock();
      */

      parent_->update(parent_->value_, parent_->grid_, parent_->cell_);

      //parent_->update(value_, grid_, cell_);
    }
    else{
      //Condition d'arrêt
      /*
      print_.lock();
      std::cerr << "ORIGIN: C'est la FIN !" << '\n';
      print_.unlock();
      */

      //grid_ = parent_->grid_;
      //cell_ = parent_->cell_;
      //end_ = true;
    }
  }
  parent_->nodeMutex_.unlock();
}

// Création de l'abre
void MinMax::createChildren(Node* parent, const Board& board, int depth){
  Board tmpBoard = board;
  if (depth && tmpBoard.gameState() == NOTHING) {
    /*
    print_.lock();

    //tmpBoard.draw();
    // * /
    std::cerr << "Am I origin : " << (parent->parent_ == nullptr) <<
                  "\nme : " << &parent <<
                  "\nmax : " << parent->max_ <<
                  "\nvalue_ : " << parent->value_ <<
                  "\ngrid_ : " << parent->grid_+1 <<
                  "\ncell_ : " << parent->cell_+1 << '\n';
    // * /
    if (parent->parent_ != nullptr) {
      std::cerr << "parent : " << parent->parent_ <<
                  "\nparent_->max : " << parent->parent_->max_ <<
                  "\nparent->value_ : " << parent->parent_->value_ <<
                  "\nparent->grid_ : " << parent->parent_->grid_+1 <<
                  "\nparent->cell_ : " << parent->parent_->cell_+1 << '\n';
    }

    print_.unlock();
    */

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

      /*
      print_.lock();
      //tmpBoard.draw();
      std::cerr << std::this_thread::get_id()  << " NODE: Je vais push l'enfant " << i+1 << "/" << tmpMaxNbChild << '\n';
      print_.unlock();
      */

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
  /*
  std::cerr << "parent->nbChild_ : " << parent_->nbChild_  <<
                "\nparent is origin : " << (parent_->parent_ == nullptr) <<
                "\nparent : " << parent_ <<
                "\nme : " << this <<
                "\nvalue_ : " << value_ <<
                "\ngrid_ : " << grid_+1 <<
                "\ncell_ : " << cell_+1 <<
                "\nparent->value_ : " << parent_->value_ <<
                "\nparent->grid_ : " << parent_->grid_+1 <<
                "\nparent->cell_ : " << parent_->cell_+1 << '\n';
  if (depth){
    symbole player;
    if(max){
      value_ = INFINITE_MIN;
      player = getSymbole();
    }
    else{
      value_ = INFINITE_MAX;
      player = opponent_;
    }


    std::vector<std::pair<int,int>> moves;

    possibleMoves(board, moves);
    nbChild_ = moves.size();
    print_.lock();
    board.draw();
    std::cerr << "parent->nbChild_ : " << parent_->nbChild_  <<
                  "\nparent is origin : " << (parent_->parent_ == nullptr) <<
                  "\nvalue_ : " << value_ <<
                  "\ngrid_ : " << grid_+1 <<
                  "\ncell_ : " << cell_+1 <<
                  "\nparent->value_ : " << parent_->value_ <<
                  "\nparent->grid_ : " << parent_->grid_+1 <<
                  "\nparent->cell_ : " << parent_->cell_+1 << '\n';
    std::cerr << "NODE: J'ai " << nbChild_ << " d'enfants"<< '\n';
    print_.unlock();
    for (int i = 0; i < nbChild_; ++i){
      Board nextMoveBoard = board;

      int nextGrid = moves[i].first;
      int nextCell = moves[i].second;

      print_.lock();
      std::cerr << "Je suis le joueur "<< player << '\n';
      print_.unlock();

      nextMoveBoard.update(player, nextGrid, nextCell);

      print_.lock();
      std::cerr << "NODE: Je vais push l'enfant " << i+1 << "/" << nbChild_ << '\n';
      print_.unlock();

      Node* me = this;

      //Push création des Nodes enfant
      pushTask([minmax, me, nextMoveBoard, depth, max, nextGrid, nextCell]//
                { Node(minmax, me, nextMoveBoard, depth-1, !max, nextGrid, nextCell); });
      //Node(this, nextMoveBoard, depth-1, !max, nextGrid, nextCell);
    }
  }
  else{
    nbChild_ = 0;
    print_.lock();
    std::cerr << "\n\nJe vais faire l'heuristique" << '\n';
    board.draw();
    std::cerr << "parent->nbChild_ : " << parent_->nbChild_  <<
                  "\nparent is origin : " << (parent_->parent_ == nullptr) <<
                  "\nparent : " << parent_ <<
                  "\nme : " << this <<
                  "\nvalue_ : " << value_ <<
                  "\ngrid_ : " << grid_+1 <<
                  "\ncell_ : " << cell_+1 <<
                  "\nparent->value_ : " << parent_->value_ <<
                  "\nparent->grid_ : " << parent_->grid_+1 <<
                  "\nparent->cell_ : " << parent_->cell_+1 << '\n';
    std::cerr << "NODE: J'ai " << nbChild_ << " d'enfants"<< '\n';
    print_.unlock();
    heuristic(board);
  }
  */
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
  /*
  std::vector<std::pair<int,int>> moves;

  possibleMoves(board, moves);
  nbChild_ = moves.size();
  for (int i = 0; i < nbChild_; ++i){
    //save coords of move in grid & cell
    Board nextMoveBoard = board;

    int nextGrid = moves[i].first;
    int nextCell = moves[i].second;

    nextMoveBoard.update(getSymbole(), nextGrid, nextCell);

    print_.lock();
    std::cerr << "ORIGIN: Je vais push l'enfant " << i+1 << "/" << nbChild_ << '\n';
    print_.unlock();

    //Node* origin = this;
    Node* origin = this;

    //Push création des Nodes enfant
    pushTask([minmax, origin, nextMoveBoard, depth, nextGrid, nextCell]
              { Node(minmax, origin, nextMoveBoard, depth-1, false, nextGrid, nextCell); });
    //Node(this, nextMoveBoard, depth, false, nextGrid, nextCell);
  }
  print_.lock();
  std::cerr << "ORIGIN: J'ai fini de push mes enfants !" << '\n';
  print_.unlock();
  */
}