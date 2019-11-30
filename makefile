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
HEADER= Player.hpp type.hpp Board.hpp Game.hpp
# liste des fichiers sources
SRC= main.cpp Player.cpp Board.cpp type.cpp Game.cpp
OBJ=$(SRC:.c=.o)

%.o: %.c $(HEADER)
	$(CC) -c $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) $(INCLUDE) $(LIB) $(OBJ) -o $@ $(FLAGS)

clear:
	rm $(EXEC)
