#ifndef ICOMPUTER_HPP
#define ICOMPUTER_HPP

#include "Player.hpp"

class IComputer : public Player {
private:
  virtual void algorithm(int& grid, int& cell) = 0;

public:
  IComputer(symbole signe, Board& board);
  bool play();
};


#endif /* end of include guard: ICOMPUTER_HPP */
