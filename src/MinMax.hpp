#ifndef MINMAX_HPP
#define MINMAX_HPP

#include "IComputer.hpp"

class MinMax : public IComputer {
private:
  void algorithm(int& grid, int& cell);
  void heuristic();
  void funcThread();

public:
  MinMax(symbole signe, Board* board);
};

#endif /* end of include guard: MINMAX_HPP */
