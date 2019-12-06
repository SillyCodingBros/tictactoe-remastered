#include "Board.hpp"

Board::Board() : curGrid_(-1), winner_(NOTHING){
  for (int i = 0; i < 81; i++) {
    board_[i] = NOTHING;
  }
}

bool Board::update(symbole signe, int grid, int cell){
  if (curGrid_ < 0 || curGrid_ == grid) {
    if (board_[grid*9+cell] == NOTHING) {
      board_[grid*9+cell] = signe;
      if (fullGrid(cell)) {
        curGrid_ = -1;
      }else{
        curGrid_ = cell;
      }
      winner_ = winner(signe, grid);
      return true;
    }
  }
  return false;
}

bool Board::fullGrid(int grid){
  for (int i = 0; i < 9; i++) {
    if (board_[grid*9+i] == NOTHING) {
      return false;
    }
  }
  return true;
}

symbole Board::winner(symbole signe, int grid){
  for (int i = 0; i < 3; i++) {
    if (board_[grid*9+i*3] == signe && board_[grid*9+i*3+1] == signe && board_[grid*9+i*3+2] == signe) {
      return signe;
    }
    if (board_[grid*9+i] == signe && board_[grid*9+i+3] == signe && board_[grid*9+i+6] == signe) {
      return signe;
    }
  }
  if (board_[grid*9] == signe && board_[grid*9+4] == signe && board_[grid*9+8] == signe) {
    return signe;
  }
  if (board_[grid*9+2] == signe && board_[grid*9+4] == signe && board_[grid*9+6] == signe) {
    return signe;
  }
  return NOTHING;
}

symbole Board::getWinner(){
  return winner_;
}

int Board::getCurGrid(){
  return curGrid_;
}

void Board::draw(){
  std::cout << '\n';
  std::cout << StoStr(board_[0]) << StoStr(board_[1]) << StoStr(board_[2]) << " | " << StoStr(board_[9]) << StoStr(board_[10]) << StoStr(board_[11]) << " | " << StoStr(board_[18]) << StoStr(board_[19]) << StoStr(board_[20]) << '\n';
  std::cout << StoStr(board_[3]) << StoStr(board_[4]) << StoStr(board_[5]) << " | " << StoStr(board_[12]) << StoStr(board_[13]) << StoStr(board_[14]) << " | " << StoStr(board_[21]) << StoStr(board_[22]) << StoStr(board_[23]) << '\n';
  std::cout << StoStr(board_[6]) << StoStr(board_[7]) << StoStr(board_[8]) << " | " << StoStr(board_[15]) << StoStr(board_[16]) << StoStr(board_[17]) << " | " << StoStr(board_[24]) << StoStr(board_[25]) << StoStr(board_[26]) << '\n';
  std::cout << "_______________\n";
  std::cout << StoStr(board_[27]) << StoStr(board_[28]) << StoStr(board_[29]) << " | " << StoStr(board_[36]) << StoStr(board_[37]) << StoStr(board_[38]) << " | " << StoStr(board_[45]) << StoStr(board_[46]) << StoStr(board_[47]) << '\n';
  std::cout << StoStr(board_[30]) << StoStr(board_[31]) << StoStr(board_[32]) << " | " << StoStr(board_[39]) << StoStr(board_[40]) << StoStr(board_[41]) << " | " << StoStr(board_[48]) << StoStr(board_[49]) << StoStr(board_[50]) << '\n';
  std::cout << StoStr(board_[33]) << StoStr(board_[34]) << StoStr(board_[35]) << " | " << StoStr(board_[42]) << StoStr(board_[43]) << StoStr(board_[44]) << " | " << StoStr(board_[51]) << StoStr(board_[52]) << StoStr(board_[53]) << '\n';
  std::cout << "_______________\n";
  std::cout << StoStr(board_[54]) << StoStr(board_[55]) << StoStr(board_[56]) << " | " << StoStr(board_[63]) << StoStr(board_[64]) << StoStr(board_[65]) << " | " << StoStr(board_[72]) << StoStr(board_[73]) << StoStr(board_[74]) << '\n';
  std::cout << StoStr(board_[57]) << StoStr(board_[58]) << StoStr(board_[59]) << " | " << StoStr(board_[66]) << StoStr(board_[67]) << StoStr(board_[68]) << " | " << StoStr(board_[75]) << StoStr(board_[76]) << StoStr(board_[77]) << '\n';
  std::cout << StoStr(board_[60]) << StoStr(board_[61]) << StoStr(board_[62]) << " | " << StoStr(board_[69]) << StoStr(board_[70]) << StoStr(board_[71]) << " | " << StoStr(board_[78]) << StoStr(board_[79]) << StoStr(board_[80]) << '\n';
  std::cout << '\n';
}
