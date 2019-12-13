#include "MinMax.hpp"

// Création d'un objet MinMax
MinMax::MinMax(symbole signe,
              symbole opponent,
              Board* board,
              int nbThread,
              int depth)
              : IComputer(signe, board),
                opponent_(opponent),
                game(true),
                depth(depth){
  //creation des threads
  for (auto i = 0; i < nbThread; i++) {
    listThread.emplace_back(&MinMax::funcThread,this,i);
  }
}

// Constructeur de task
MinMax::task::task(Board board, node* curNode, int depth, bool up) : board(board), curNode(curNode), depth(depth), up(up){
}

// Constructeur vide de task
MinMax::task::task(){
}

// Constructeur vide de node
MinMax::node::node(){
}

MinMax::node::node(int grid,
                  int cell,
                  int value,
                  bool max,
                  node* parent)
                  : grid(grid),
                    cell(cell),
                    value(value),
                    max(max),
                    parent(parent){
}

// Fonction pour charger les attributs du noeud originel
void MinMax::node::loadOrigin(int minValue){
  value = minValue;
  max = true;
  parent = nullptr;
}

/*
// Algorithme de choix de coup minmax
void MinMax::algorithm(int& grid, int& cell) {
  //je sais pas si c'est necessaire
  std::unique_lock<std::mutex> lock(mutex_cond_var);

  for (int i = 0; i < jobDone.size(); i++) {
    jobDone[i] = false;
  }
  node origin;
  Board tmp = *board_;
  origin.max = true;
  std::vector<std::pair<int,int>> move;
  int gridMove = tmp.getCurGrid();
  int nbGrid = gridMove+1;
  if (grid == -1) {
    gridMove = 0;
    nbGrid = 9;
  }
  for (; gridMove < nbGrid; gridMove++) {
    for (int cellMove = 0; cellMove < 9; cellMove++) {
      if (tmp.getCell(gridMove*9+cellMove) == NOTHING) {
        move.emplace_back(gridMove,cellMove);
      }
    }
  }
  origin.workChild = move.size();
  for (int i = 0; i < move.size(); i++) {
    if (!tmp.update(symbole_,move[i].first,move[i].second)) {
      std::cerr << "error IA" << '\n';
      return ;
    }
    emplaceTask(tmp,&origin,depth,false);
    std::cout << "lancement des threads" << '\n';
  }
  //attendre la fin des taches
  bool done = true;
  while (done) {
    for (int i = 0; i < jobDone.size(); i++) {
      if (!jobDone[i]) break ;
      done = false;
    }
  }
  grid = origin.grid;
  cell = origin.cell;
}
*/

// Algorithme de choix de coup minmax
void MinMax::algorithm(int& grid, int& cell) {
  node origin;
  origin.loadOrigin(minValue);
  std::vector<std::pair<int,int>> move;
  Board boardCopie = *board_;
  possibleMove(boardCopie, move);
  origin.workChild = move.size();
  for (size_t i = 0; i < move.size(); i++) {
    Board tmp = boardCopie;
    node* childOrigin = new node(move[i].first,
                                move[i].second,
                                maxValue,
                                false,
                                &origin);
    tmp.update(symbole_, move[i].first, move[i].second);
    print.lock();
    std::cout << "origin emplace" << '\n';
    print.unlock();
    emplaceTask(tmp, childOrigin, depth, false);
  }
  // Attente de la fin des tâches
  while (true) {
    if (origin.workChild == 0) {
      break;
    }
  }
  grid = origin.grid;
  cell = origin.cell;
}

// Ajoute une tâche à la fin de la queue
void MinMax::pushTask(task& task){
  print.lock();
  std::cout << "je push" << '\n';
  print.unlock();
  taskMutex.lock();
  taskQueue.push(task);
  taskMutex.unlock();
  print.lock();
  std::cout << "j'ai push" << '\n';
  print.unlock();
  //cond_var.notify_one();
}

// Construit une tâche au début de la queue
void MinMax::emplaceTask(Board& board, node* curNode, int depth, bool up){
  print.lock();
  std::cout << "j'emplace" << '\n';
  print.unlock();
  taskMutex.lock();
  taskQueue.emplace(board, curNode, depth, up);
  taskMutex.unlock();
  print.lock();
  std::cout << "ai fini !!!!!" << '\n';
  print.unlock();
  //cond_var.notify_one();
}

// Gestion des threads
void MinMax::funcThread(int id){

  std::unique_lock<std::mutex> lock(mutex_cond_var);
  task task;

  //boucle tant que la partie existe
  while (game) {
    //attendre les taches
    while (taskQueue.empty()) {
      //cond_var.wait(lock);
    }
    while (!taskQueue.empty()){
      // Récupération de la tâche à faire
      taskMutex.lock();
      task = taskQueue.front();
      taskQueue.pop();
      taskMutex.unlock();
      if (task.up) {
        // Remonter les résultats des feuilles jusqu'à la racine suivant minmax
        if (task.curNode->workChild != 0) {
          print.lock();
          std::cout << "push parce que peut pas remonter : "
                    << task.curNode->workChild << '\n';
          std::cout << "queue.size = " << taskQueue.size() << '\n';
          std::queue<MinMax::task> tmp = taskQueue;
          while(!tmp.empty()){
            MinMax::task taskTmp = tmp.front();
            std::cout << "task " << " WC " << taskTmp.curNode->workChild << " val " << taskTmp.curNode->value << " max " << taskTmp.curNode->max << " depth " << taskTmp.depth << " parent " << taskTmp.curNode->parent << '\n';
            tmp.pop();
          }
          print.unlock();
          pushTask(task);
        }else{
          if (task.curNode->parent->max) {
            if (task.curNode->value > task.curNode->parent->value) {
              task.curNode->parent->value = task.curNode->value;
            }
          }else{
            if (task.curNode->value < task.curNode->parent->value) {
              task.curNode->parent->value = task.curNode->value;
            }
          }
          task.curNode->parent->workChild--;
          std::cout << "decr : " << task.curNode->parent->workChild << '\n';
          if (task.curNode->parent->parent != nullptr) {
            task.curNode = task.curNode->parent;
            print.lock();
            std::cout << "push pour remonter les nodes au parent" << '\n';
            print.unlock();
            assert(task.curNode->parent->workChild >= 0);
            pushTask(task);
          }
        }
      }else{
        // Construire les branches de l'arbre de profondeur n
        if (task.board.gameState() != NOTHING || task.depth == 0){
          // Évaluer les feuilles car la branche est terminale
          //(game-over/depth_max)
          task.curNode->value = heuristic(&task.board);
          task.curNode->workChild = 0;
          task.depth = 0;
          task.up = true;
          print.lock();
          std::cout << "push pour remonter heuristic" << '\n';
          print.unlock();
          assert(task.curNode->workChild >= 0);
          pushTask(task);
        }else{
          // La branche n'est pas terminale
          std::vector<std::pair<int,int>> move;
          possibleMove(task.board, move);
          print.lock();
          std::cout << "juste apres move.size() = " << move.size() << '\n';
          print.unlock();
          for (size_t i = 0; i < move.size(); i++) {
            Board tmp = task.board;
            node* child;
            symbole tmpSymbol;
            if (task.curNode->max) {
              //enfant min
              child = new node(task.curNode->grid,
                              task.curNode->cell,
                              minValue,
                              false,
                              task.curNode);
              tmpSymbol = symbole_;
            }else{
              //enfant max
              child = new node(task.curNode->grid,
                              task.curNode->cell,
                              maxValue,
                              true,
                              task.curNode);
              tmpSymbol = opponent_;
            }
            child->workChild = move.size();
            tmp.update(tmpSymbol, move[i].first, move[i].second);
            print.lock();
            std::cout << "child->workChild = " << child->workChild << '\n';
            print.unlock();
            assert(child->workChild >= 0);
            emplaceTask(tmp, child, task.depth-1, false);
            move.clear();
          }
        }
      }
    }
  }
}

/*
// Gestion des threads
void MinMax::funcThread(int id){
  std::unique_lock<std::mutex> lock(mutex_cond_var);
  task task;
  while (game){
    jobDone[id] = true;
    while (taskQueue.empty()){
      cond_var.wait(lock);
    }
    jobDone[id] = false;
    while (!taskQueue.empty()) {
      taskMutex.lock();
      task = taskQueue.front();
      taskQueue.pop();
      taskMutex.unlock();
      if (task.up) {
        if (task.curNode->parent == nullptr) {
          continue;
        }
        if (task.curNode->workChild != 0) {
          //push la tache parce que peut pas encore traiter
          pushTask(task);
        }
        task.curNode->parent->nodeMutex.lock();
        if (task.curNode->parent->max) {
          if (task.curNode->value > task.curNode->parent->value) {
            task.curNode->parent->value = task.curNode->value;
            task.curNode->parent->grid = task.curNode->grid;
            task.curNode->parent->cell = task.curNode->cell;
          }
        }else{
          if (task.curNode->value < task.curNode->parent->value) {
            task.curNode->parent->value = task.curNode->value;
            task.curNode->parent->grid = task.curNode->grid;
            task.curNode->parent->cell = task.curNode->cell;
          }
        }
        task.curNode->parent->workChild--;
        task.curNode->parent->nodeMutex.unlock();
        //push une noouvelle tache avec le parent
        emplaceTask(task.board,task.curNode->parent,task.depth,true);
      }
      if (task.depth == 0) {
        task.curNode->value = heuristic(&task.board);
        //push une nouvelle tache pour remonter dans l'arbre
        task.up = true;
        pushTask(task);
      }else{
        node* child = new node;
        child->parent = task.curNode;
        if (task.curNode->max) {
          child->max = false;
          child->value = minValue;
        }else{
          child->max = true;
          child->value = maxValue;
        }
        std::vector<std::pair<int,int>> move;
        int grid = task.board.getCurGrid();
        int nbGrid = grid + 1;
        if (grid == -1) {
          grid = 0;
          nbGrid = 9;
        }
        for (; grid < nbGrid; grid++) {
          for (int cell = 0; cell < 9; cell++) {
            if (task.board.getCell(grid * 9 + cell) == NOTHING) {
              move.emplace_back(grid,cell);
            }
          }
        }
        child->workChild = move.size();
        for (int i = 0; i < move.size(); i++) {
          Board tmp = task.board;
          if (!tmp.update(symbole_,move[i].first,move[i].second)) {
            std::cerr << "error IA" << '\n';
            return ;
          }
          if (tmp.gameState() == NOTHING) {
            //creer une tache qui continuera de creer l'arbre
            emplaceTask(tmp,child,task.depth-1,false);
          }else{
            //créer une tache feuille (fin de la creation)
            emplaceTask(tmp,child,0,false);
          }
        }
      }
    }
  }
}
*/

// Heuristique de l'algorithme
int MinMax::heuristic(Board* board){
  int sum = 0;
  for (auto i=0; i < 9; ++i){
    for (auto j=0; j < 8; ++j){
      sum += evaluateLine(caseValue(board->getCell(POSSIBILITIES[0 + (j * 3)] + (9 * i))) +
                           caseValue(board->getCell(POSSIBILITIES[1 + (j * 3)] + (9 * i))) +
                           caseValue(board->getCell(POSSIBILITIES[2 + (j * 3)] + (9 * i))));
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
    return maxValue;
  }
  else if (line == -6){
    return minValue;
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
        std::cout << "case" << curGrid << "," << i << "ok" << '\n';
        print.unlock();
        move.emplace_back(curGrid, i);
      }
    }
  }
}
