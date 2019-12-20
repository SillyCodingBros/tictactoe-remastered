#ifndef TYPE
#define TYPE

typedef enum {
  NOTHING,
  CROSS,
  CIRCLE,
  TIE,
} symbole;

typedef enum {
  PVP,
  PVRANDOM,
  PVAI,
  AIVAI,
  RANDOMVAI,
} gamemode;

char StoStr(symbole signe);

#endif /* end of include guard: TYPE */
