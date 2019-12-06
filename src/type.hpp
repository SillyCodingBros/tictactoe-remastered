#ifndef TYPE
#define TYPE

typedef enum {
  NOTHING,
  CROSS,
  CIRCLE,
} symbole;

typedef enum {
  PVP,
  PVAUPIF,
} gamemode;

char StoStr(symbole signe);

#endif /* end of include guard: TYPE */
