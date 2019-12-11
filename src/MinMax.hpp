#ifndef MINMAX_HPP
#define MINMAX_HPP

#include <climits>
#include <ctime>

#include "IComputer.hpp"

class MinMax : public IComputer {
private:
  void algorithm(int& grid, int& cell);
  //int heuristic(Board* board);
  void funcThread();
  int heuristic(Board* board);
  int evaluate_line(int LINE);
  int case_value(symbole cell);
  int MAX_VALUE = INT_MAX;
  int POSSIBILITIES[24] = {0, 1, 2,
                           3, 4, 5,
                           6, 7, 8,
                           0, 3, 6,
                           1, 4, 7,
                           2, 5, 8,
                           0, 4, 8,
                           2, 4, 6};

public:
  MinMax(symbole signe, Board* board);
};

#endif /* end of include guard: MINMAX_HPP */
