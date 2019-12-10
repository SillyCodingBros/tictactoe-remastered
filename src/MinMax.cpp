#include "MinMax.hpp"

MinMax::MinMax(symbole signe, Board* board) : IComputer(signe, board){
  //creation des threads
}

void MinMax::algorithm(int& grid, int& cell) {

}

void MinMax::heuristic(){

}

void MinMax::funcThread(){
  //fonction pour les threads

  //boucle tant que la partie existe
  //attendre les taches a faire depuis la pile
  //get une tache puis la suppr
  //si feuille alors heuristic sinon min/max resultat des enfants
  //mettre le resultat de la tache dans l'emplacement memoire dedié
}
