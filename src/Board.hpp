#ifndef BOARD
#define BOARD

#include <iostream>

#include "type.hpp"

class Board {
private:
  symbole board_[81];
  int curGrid_;
  //symbole winner_;
  int POSSIBILITIES[24] = {0, 1, 2,
                           3, 4, 5,
                           6, 7, 8,
                           0, 3, 6,
                           1, 4, 7,
                           2, 5, 8,
                           0, 4, 8,
                           2, 4, 6};

public:
  Board();
  bool update(symbole signe,int grid, int cell);
  bool fullBoard();
  bool fullGrid(int grid);
  symbole gameState();
  //symbole winner(symbole signe, int grid);
  //symbole getWinner();
  int getCurGrid();
  symbole getCell(int pos);
  void draw();
};

#endif /* end of include guard: BOARD */
