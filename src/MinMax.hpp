#ifndef MINMAX_HPP
#define MINMAX_HPP



class MinMax : public IComputer {
private:
  void algorithm(int& grid, int& cell);
  void heuristic();

public:
  MinMax(symbole signe, Board* board);
};


#endif /* end of include guard: MINMAX_HPP */
