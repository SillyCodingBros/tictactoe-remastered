#include "type.hpp"

// Retourne le caractère associé au symbole donné
char StoStr(symbole signe){
  switch (signe) {
    case NOTHING:
      return ' ';
    case CROSS:
      return 'X';
    case CIRCLE:
      return 'O';
    case TIE:
      return '=';
    default:
      return '#';
  }
}
