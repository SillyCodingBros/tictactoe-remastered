#include "Player.hpp"

Player::Player() : symbole_(NOTHING), board_(nullptr){
}

Player::Player(symbole signe, Board* board) : symbole_(signe), board_(board){
}

bool Player::play(int grid, int cell){
  return board_->update(symbole_,grid,cell);
}

symbole Player::getSymbole(){
  return symbole_;
}
