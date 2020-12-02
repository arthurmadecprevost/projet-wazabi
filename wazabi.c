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

typedef struct DefCarte{
    char libelle[75];
    int nbWasabi;
} DefCarte;

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
	int id;  //Numéro du joueur
    char pseudo[25]; //Pseudo du joueur
    TCarte * cartes; //Les cartes du joueur
    TDe * des; //Les dés du joueur
    struct TJoueur * joueurPrec; //Le joueur précédent
    struct TJoueur * joueurSuiv; //Le joueur suivant
    bool joue;	//
} TJoueur;
typedef struct _TListeJoueur
{
    TJoueur * debut; //Liste des joueurs
}TListeJoueur;

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
void init_pioche(TPioche * pioche); // Initialisation de la pioche.
int nombre_des(TJoueur leJoueur); // Fonction qui retourne le nombre de dés du joueur passé en paramètre.
void init_partie(TListeJoueur * listeJoueur, TPioche * pioche); // Procédure qui initialise les dés, les cartes ainsi que le pseudo d’un joueur
void ajout_joueur(TListeJoueur * listeJoueur,int numJoueur, TPioche * pioche); //Procédure qui ajoute un joueur à la liste des joueurs  
void afficher_carte(TCarte * laCarte, DefCarte tabCarte[10]); //procédure qui affiche une carte
void defausser_carte(TCarte * laCarte, TJoueur * leJoueur, TPioche * defausse);// procédure qui va mettre une carte de la main d'un joueur dans la defausse 
void tour_suivant( TJoueur * leJoueur,bool sens); //procédure qui va changer le joueur actuel
void init_tabCarte(DefCarte tabCarte[10]); //procédure qui va initialiser le tableau de définitions des cartes

// **********************
//  programme principal
// **********************
int main ()
{
    int nb;
    TListeJoueur *listeJoueur;
    TPioche *pioche;
    srand(time(NULL));
    init_partie(listeJoueur,pioche);   
    //Le tableau d'enregistrement des cartes 
    DefCarte tabCarte[10];

    TJoueur * unJoueur;
    
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

    (*pioche).nbCarte = 36;
    (*pioche).tabPioche = tabPioche;
}

void piocher_carte(TJoueur * leJoueur, TPioche * pioche){
    TCarte * newCarte;
    newCarte = (*pioche).sommet;
    (*pioche).sommet = (*newCarte).carteSuivante;
    
    TCarte * aux;

    
    (*newCarte).carteSuivante=NULL;


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
}
//Procédure qui ajoute un joueur à la partie
void ajout_joueur(TListeJoueur * listeJoueur, int numJoueur, TPioche * pioche){ 
    TJoueur *newJoueur = NULL;
    TJoueur *aux = NULL;

    newJoueur = malloc(sizeof(newJoueur));
    (*newJoueur).id = numJoueur;
    printf("Joueur n°%d saisissez votre pseudo \n", numJoueur);
    scanf(" %s", (*newJoueur).pseudo);
    (*newJoueur).joue = false;
    for(int i = 0; i < 4; i++)//initialisation des cartes du joueur
    {
        piocher_carte(newJoueur,pioche);
    }
    init_de(newJoueur);
    (*newJoueur).joueurPrec = NULL;
    (*newJoueur).joueurSuiv = NULL;

    if((*listeJoueur).debut == NULL)//Si c'est le premier joueur
    {
        (*listeJoueur).debut = newJoueur;
    } 
    else
    {
        aux = (*listeJoueur).debut;
        while ((*aux).joueurSuiv != NULL)
        {
            aux = (*aux).joueurSuiv;
        }
        (*aux).joueurSuiv = newJoueur;
        (*newJoueur).joueurPrec = aux;
        if(numJoueur == 3) //Si c'est le dernier joueur alors il pointe vers le 1er et inversement
        {
            (*newJoueur).joueurSuiv = (*listeJoueur).debut;
            (*listeJoueur).debut->joueurPrec = newJoueur;
        }
    }
}
void init_partie(TListeJoueur * listeJoueur, TPioche * pioche){
    int i;
    printf("Bienvenue dans le jeu Wazabi");
    for(i = 0; i < 3; i++)
    {
        ajout_joueur(listeJoueur,i,pioche);
    }
} 

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

void afficher_carte(TCarte * laCarte, DefCarte tabCarte[10]){
    printf("Nombre de wasabi : %d : %s ",tabCarte[(*laCarte).identifiant].nbWasabi,tabCarte[(*laCarte).identifiant].libelle);
}

void defausser_carte(TCarte * laCarte, TJoueur * leJoueur, TPioche * defausse){
    bool supprimer = false;
    bool premier = true;
    TCarte * aux;
    TCarte * prec;

    aux = (*leJoueur).cartes;
    prec = aux;
    while (aux != NULL && !supprimer){
        if((*aux).identifiant == (*laCarte).identifiant){
            if(premier){
                (*leJoueur).cartes=(*aux).carteSuivante; 
            }
            (*prec).carteSuivante=(*aux).carteSuivante;
            (*aux).carteSuivante = (*defausse).sommet;
            (*defausse).sommet = aux;
        }
        premier = false;
        prec= aux;
        aux=(*aux).carteSuivante;
    }
}

void tour_suivant(TJoueur * leJoueur,bool sens){
    (*leJoueur).joue = false;
    TJoueur * nouveauJ;

    if(sens){
        nouveauJ = (*leJoueur).joueurSuiv;
        (*nouveauJ).joue = true;
    }else{
        nouveauJ = (*leJoueur).joueurPrec;
        (*nouveauJ).joue = true;
    }
}

void init_tabCarte(DefCarte tabCarte[10]){
    tabCarte[0].libelle[75] = "Supprimez 1 de vos des ";
    tabCarte[1].libelle[75] = "Tous les joueurs donnent leurs des a leur voisin de gauche ou de droite ";
    tabCarte[2].libelle[75] = "Supprimez 2 de vos des ";
    tabCarte[3].libelle[75] = "Donnez 1 de vos des au joueur de votre choix ";
    tabCarte[4].libelle[75] = "Prenez 1 carte au joueur de votre choix ";
    tabCarte[5].libelle[75] = "Le joueur de votre choix n’a plus qu’1 carte ";
    tabCarte[6].libelle[75] = "Piochez 3 cartes dans la pioche ";
    tabCarte[7].libelle[75] = "Tous les joueurs sauf vous n’ont plus que 2 cartes";
    tabCarte[8].libelle[75] = "Le joueur de votre choix passe son tour";
    tabCarte[9].libelle[75] = "Rejouez et changez de sens ";
   
    tabCarte[0].nbWasabi = 1;
    tabCarte[0].nbWasabi = 2;
    tabCarte[0].nbWasabi = 3;
    tabCarte[0].nbWasabi = 3;
    tabCarte[0].nbWasabi = 1;
    tabCarte[0].nbWasabi = 1;
    tabCarte[0].nbWasabi = 1;
    tabCarte[0].nbWasabi = 2;
    tabCarte[0].nbWasabi = 0;
    tabCarte[0].nbWasabi = 0;
}

