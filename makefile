# Compilateur
CC = g++

# Options pour la création des fichiers objets
CFLAGS = -Wall -g

# Options pour la création de l'exécutable
FLAGS =

# Add include
INCLUDE =

# Add lib
LIB =

# Nom de l'exécutable
EXEC = test.out

# Liste des fichiers d'en-tête
HEADER = src/Player.hpp src/type.hpp src/Board.hpp src/Game.hpp src/IComputer.hpp src/Random.hpp

# Liste des fichiers sources
SRC = src/main.cpp src/Player.cpp src/Board.cpp src/type.cpp src/Game.cpp src/IComputer.cpp src/Random.cpp

OBJ = $(SRC:.c=.o)

%.o: %.c $(HEADER)
	$(CC) -c $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) $(INCLUDE) $(LIB) $(OBJ) -o $@ $(FLAGS)

clear:
	rm $(EXEC)
