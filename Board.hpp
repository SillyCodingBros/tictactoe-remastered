#ifndef BOARD
#define BOARD

#include <iostream>

#include "type.hpp"

class Board {
private:
  symbole board_[81];
  int curGrid_;
  symbole winner_;

public:
  Board();
  bool update(symbole signe,int grid, int cell);
  void draw();
};

#endif /* end of include guard: BOARD */
