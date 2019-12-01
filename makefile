# compilateur
CC=g++
# option pour la creation des fichier objet
CFLAGS= -Wall
# option pour la creation de l'executable
FLAGS=
# add include
INCLUDE=
# add lib
LIB=
# nom de l'executable
EXEC= test
# liste des fichiers d'en-tete
HEADER= src/Player.hpp src/type.hpp src/Board.hpp src/Game.hpp
# liste des fichiers sources
SRC= src/main.cpp src/Player.cpp src/Board.cpp src/type.cpp src/Game.cpp
OBJ=$(SRC:.c=.o)

%.o: %.c $(HEADER)
	$(CC) -c $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) $(INCLUDE) $(LIB) $(OBJ) -o $@ $(FLAGS)

clear:
	rm $(EXEC)
