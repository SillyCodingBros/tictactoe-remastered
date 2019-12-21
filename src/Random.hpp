#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <ctime>
#include <cstdlib>

#include "IComputer.hpp"

class Random : public IComputer {
private:
  void algorithm(int& grid, int& cell);

public:
  Random(symbole signe, Board& board);
};

#endif /* end of include guard: RANDOM_HPP */