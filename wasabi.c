#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "carte.h"
#include "de.h"
#include "init.h"
#include "game.h"

// ************************************
// definition des types : liste chainee 
// ************************************
typedef struct _TPioche
{
    int nbCarteRestante;
    int tabPioche[36];
} TPioche;

typedef struct _TCarte
{
    char libelle[75];
    int cout;
    int effet;
    struct _TCarte * carteSuivante;
} TCarte;

typedef struct TDe
{
    int valeur;
    struct _TDe * deSuivant;
} TDe;

typedef struct TJoueur
{
	int id;
    char pseudo[25];
    TCarte *  cartes;
    TDe * des;
    struct TJoueur * joueurPrec;
    struct TJoueur * joueurSuiv;
    bool joue;	
} TJoueur;




// **********************
//  prototypes des fonctions
// **********************
int saisir_entre(int min, int max); 
int nombre_aleatoire(int min, int max);

// **********************
//  programme principal
// **********************
int main ()
{
    int nb;
    srand(time(NULL));

    nb = nombre_aleatoire(1,10);
    printf(" %d \n", nb);
    
    return 0;
}

// fonction pour saisir un nombre entre min et max
int saisir_entre(int min, int max){
 	int choix;
    do{
        printf("Veuillez saisir un chiffre entre %d et %d : \n", min, max);
        scanf("%d", &choix);
    }while(choix<min || choix>max);

    return choix;
}
// fonction qui retourne un nombre aléatoire compris entre deux nombres
int nombre_aleatoire(int min, int max){
    int nbMystere;

    nbMystere = (rand() % (max-min - 1)) + min;

    return nbMystere;
}
