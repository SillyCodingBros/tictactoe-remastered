#include "type.hpp"

char StoStr(symbole signe){
  switch (signe) {
    case NOTHING:
      return ' ';
    case CROSS:
      return 'X';
    case CIRCLE:
      return 'O';
    default:
      return '#';
  }
}
