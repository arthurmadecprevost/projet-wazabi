#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
//#include "carte.h"
//#include "init.h"
//#include "de.h"
//#include "game.h"

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

typedef struct _TDe
{
    int valeur;
    struct _TDe * deSuivant;
} TDe;

typedef struct _TJoueur
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
void init_pioche(TPioche * pioche); // Initialisation de la pioche.
void piocher_carte(TJoueur * leJoueur, TPioche pioche); // Procédure qui va prendre un numéro dans la pioche, créer la carte et la mettre dans le deck des joueurs.
void init_de(TJoueur * leJoueur);// Procédure qui va donner 4 dés à un joueur et initialiser leur valeur à -1
int nombre_des(TJoueur leJoueur); // Fonction qui retourne le nombre de dés du joueur passé en paramètre.


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

void init_pioche(TPioche * pioche){
    int tabPioche[36] = {1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 9, 10, 10};

    (*pioche).nbCarteRestante = 36;
    (*pioche).tabPioche = tabPioche;
}

void piocher_carte(TJoueur * leJoueur, TPioche pioche){
    int numCarte = nombre_aleatoire(0,pioche.nbCarteRestante); //numéro de la carte à piocher dans le tableau 
    int laCarte ;
    int i =0; //utiliser pour parcourir le tableau
    bool trouve = false;
    
    while((i<35) && !trouve){
        int compteur = 0; //On va augmenter la valeur de compteur quand on compte une carte toujours présente dans la pioche donc dont la valeur n'est pas égal à -1
        if(pioche.tabPioche[i]!= -1){
            compteur = compteur + 1;
        }
        if(compteur == numCarte){
            laCarte = pioche.tabPioche[i];
            trouve = true;
        }
    }

    char carte1[75] = "Supprimez 1 de vos des ";
    char carte2[75] ="Tous les joueurs donnent leurs des a leur voisin de gauche ou de droite. ";
    char carte3[75] ="Supprimez 2 de vos des ";
    char carte4[75] ="Donnez 1 de vos des au joueur de votre choix ";
    char carte5[75] ="Prenez 1 carte au joueur de votre choix ";
    char carte6[75] ="Le joueur de votre choix n’a plus qu’1 carte ";
    char carte7[75] ="Piochez 3 cartes dans la pioche ";
    char carte8[75] ="Tous les joueurs sauf vous n’ont plus que 2 cartes";
    char carte9[75] ="Le joueur de votre choix passe son tour";
    char carte10[75] ="Rejouez et changez de sens ";

    TCarte * newCarte;
    newCarte = (TCarte*) malloc(sizeof(TCarte));
    switch (laCarte)
    {
    case 1:
        (*newCarte).cout=1;
        strcpy((*newCarte).libelle,carte1);
        (*newCarte).effet=1;
        break;
    case 2:
        (*newCarte).cout=2;
        strcpy((*newCarte).libelle,carte1);
        (*newCarte).effet=2;
        break;
    case 3:
        (*newCarte).cout=3;
        strcpy((*newCarte).libelle,carte1);
        (*newCarte).effet=3;
        break;
    case 4:
        (*newCarte).cout=3;
        strcpy((*newCarte).libelle,carte1);
        (*newCarte).effet=4;
        break;
    case 5:
        (*newCarte).cout=1;
        strcpy((*newCarte).libelle,carte1);
        (*newCarte).effet=5;
        break;
    case 6:
        (*newCarte).cout=1;
        strcpy((*newCarte).libelle,carte1);
        (*newCarte).effet=6;
        break;
    case 7:
        (*newCarte).cout=1;
        strcpy((*newCarte).libelle,carte1);
        (*newCarte).effet=7;
        break;
    case 8:
        (*newCarte).cout=2;
        strcpy((*newCarte).libelle,carte1);
        (*newCarte).effet=8;
        break;
    case 9:
        (*newCarte).cout=0;
        strcpy((*newCarte).libelle,carte1);
        (*newCarte).effet=9;
        break;
    case 10:
        (*newCarte).cout=0;
        strcpy((*newCarte).libelle,carte1);
        (*newCarte).effet=10;
        break;
    default:
        break;
    }

    (*newCarte).carteSuivante=NULL;

    TCarte * aux;

    if((*leJoueur).cartes==NULL){
        (*leJoueur).cartes = newCarte;
    }else{
        aux= (*leJoueur).cartes;
        while (aux!=NULL)
        {
            if((*aux).carteSuivante==NULL){
                (*aux).carteSuivante = newCarte;
            }
            aux =(*aux).carteSuivante;
        }
    }
}

void init_de(TJoueur * leJoueur){
    for(int i =0; i<4;i++){
        nouveau_de(leJoueur);
    }
}

int nombre_des(TJoueur leJoueur){

}