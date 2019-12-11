#include "Player.hpp"

// Création d'un objet Player
Player::Player(symbole signe, Board* board) : symbole_(signe), board_(board){
}

// Retourne le symbole associé au joueur
symbole Player::getSymbole(){
  return symbole_;
}

// Création d'un objet User
User::User(symbole signe, Board* board) : Player(signe, board){
}

// Affichage de la demande de choix du prochain coup
void User::display(int* gridInput, int* cellInput, bool* cond) {
  if (board_->getCurGrid() == -1) {
    std::cout << "Player " << symbole_ << " choose a grid (1-9) and a cell (1-9)" << '\n';
    std::cout << "Enter 'number number': ";
    std::cin >> *gridInput >> *cellInput;
    *gridInput -= 1;
    *cellInput -= 1;
    *cond = *gridInput < 0 || *gridInput > 8 || *cellInput < 0 || *cellInput > 8;
  }
  else{
    std::cout << "Playing on grid " << *gridInput+1 << '\n';
    std::cout << "Player " << symbole_ << " choose a cell (1-9)" << '\n';
    std::cout << "Enter 'number': ";
    std::cin >> *cellInput;
    *cellInput -= 1;
    *cond = *cellInput < 0 || *cellInput > 8;
  }
}

// Jouer un coup
bool User::play(){
  int gridInput = board_->getCurGrid();
  int cellInput;
  bool cond;

  display(&gridInput, &cellInput, &cond);

  while (cond) {
    std::cout << "Bad input" << '\n';
    display(&gridInput, &cellInput, &cond);
  }

  return board_->update(symbole_,gridInput,cellInput);
}
