#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
//#include "carte.h"
//#include "init.c"
//#include "de.h"
//#include "game.h"

// ************************************
// definition des types : liste chainee 
// ************************************
typedef struct _TCarte
{
    int identifiant;
    struct _TCarte * carteSuivante;
} TCarte;

typedef struct _TPioche
{
    TCarte * sommet;
} TPioche;

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
void piocher_carte(TJoueur * leJoueur, TPioche * pioche); //Procédure qui va prendre un numéro dans la pioche, créer la carte et la mettre dans le deck des joueurs
void init_de(TJoueur * leJoueur);// Procédure qui va donner 4 dés à un joueur et initialiser leur valeur à -1
void afficher_les_joueurs(TJoueur * leJoueur1,TJoueur * leJoueur2,TJoueur * leJoueur3);//procédure qui afficher tous les joueurs à l'écran
void afficher_lancer(TJoueur * leJoueur);// procédure qui affiche les dés d'un joueur
void echange_de(TJoueur * leJoueur1,TJoueur * leJoueur2,TJoueur * leJoueur3);// Procédure qui va demander au joueur dans quel sens tourner les dés et ensuite faire l’échange des dés entre les joueurs
void egaliser_de(TJoueur * leJoueur,int nbDeDeb,int nbDeNouveau);//Procédure qui va faire en sorte que le joueur ai un certains nombre de dé



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

void piocher_carte(TJoueur * leJoueur, TPioche * pioche){
    TCarte * newCarte;
    newCarte = (*pioche).sommet;
    (*pioche).sommet = (*newCarte).carteSuivante;
    
    TCarte * aux;

    
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
    (*newCarte).carteSuivante = NULL;


    // char carte1[75] = "Supprimez 1 de vos des ";
    // char carte2[75] ="Tous les joueurs donnent leurs des a leur voisin de gauche ou de droite. ";
    // char carte3[75] ="Supprimez 2 de vos des ";
    // char carte4[75] ="Donnez 1 de vos des au joueur de votre choix ";
    // char carte5[75] ="Prenez 1 carte au joueur de votre choix ";
    // char carte6[75] ="Le joueur de votre choix n’a plus qu’1 carte ";
    // char carte7[75] ="Piochez 3 cartes dans la pioche ";
    // char carte8[75] ="Tous les joueurs sauf vous n’ont plus que 2 cartes";
    // char carte9[75] ="Le joueur de votre choix passe son tour";
    // char carte10[75] ="Rejouez et changez de sens ";

}

void init_de(TJoueur * leJoueur){
    for(int i =0; i<4;i++){
        nouveau_de(leJoueur);
    }
}

void afficher_les_joueurs(TJoueur * leJoueur1,TJoueur * leJoueur2,TJoueur * leJoueur3){
    afficher_joueur(leJoueur1);
    afficher_joueur(leJoueur2);
    afficher_joueur(leJoueur3);
}

void afficher_lancer(TJoueur * leJoueur){
    TDe * aux;
    aux = (TDe*) malloc(sizeof(TDe));
    int valeurDe;

    printf("Voici vos dés : ");

    aux = (*leJoueur).des;
    while(aux != NULL){
        valeurDe = (*aux).valeur;
        switch (valeurDe)
        {
        case 1:
            printf("Wasabi");
            break;
        case 2:
            printf("Donner dé");
            break;
        case 3:
            printf("Piocher carte");
            break;
        default:
            break;
        }
        printf(" - ");
        aux=(*aux).deSuivant;
    }
}

void echange_de(TJoueur * leJoueur1,TJoueur * leJoueur2,TJoueur * leJoueur3){
    printf("Dans quelle sens voulez vous tournez les dés ? (0 pour sens horaire, 1 pour sens anti horaire)\n ");
    int sens = saisir_entre(0,1);
    int de1 = nombre_de(leJoueur1);
    int de2 = nombre_de(leJoueur2);
    int de3 = nombre_de(leJoueur3);
    int dif = 0;

    if(sens){
        egaliser_de(leJoueur1,de1,de3);
        egaliser_de(leJoueur2,de2,de1);
        egaliser_de(leJoueur3,de3,de2);
    }else{
        egaliser_de(leJoueur1,de1,de2);
        egaliser_de(leJoueur2,de2,de3);
        egaliser_de(leJoueur3,de3,de1);    
    }

}

void egaliser_de(TJoueur * leJoueur,int nbDeDeb,int nbDeNouveau){
    int nbDeAct = nbDeDeb;
    while (nbDeAct != nbDeNouveau){
        if(nbDeAct>nbDeNouveau){
            supprimer_de(leJoueur);
            nbDeAct = nbDeAct -1;
        }else{
            nouveau_de(leJoueur);
            nbDeAct = nbDeAct +1;
        }
    }
}

int nombre_des(TJoueur leJoueur){
    
}

void melanger_carte(TPioche * laPioche, TPioche * laDefausse){
    int valeur = 0;
    int nbCarteDef = 0;
    int parcours =1;

    TCarte * aux;
    TCarte * prec;

    aux = (*laDefausse).sommet;
    while(aux != NULL){
        nbCarteDef = nbCarteDef+1;
        aux = (*aux).carteSuivante;
    }

    while((*laDefausse).sommet != NULL){
        aux = (*laDefausse).sommet;

        valeur = nombre_aleatoire(1,nbCarteDef);
        while(parcours != valeur){
            prec = aux;
            aux = (*aux).carteSuivante;
            valeur = valeur +1;
        }
        (*prec).carteSuivante=(*aux).carteSuivante;
        if((*laPioche).sommet==NULL){
            (*laPioche).sommet = aux;
            (*aux).carteSuivante = NULL;
        }else{
            (*aux).carteSuivante = (*laPioche).sommet;
            (*laPioche).sommet = aux;
        }
    }
}