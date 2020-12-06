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
    char * libelle[75];
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
    bool joue;	//
} TJoueur;



// **********************
//  prototypes des fonctions
// **********************
void init_partie(TJoueur tabJoueur[], TPioche * pioche, TPioche * defausse); // Procédure qui initialise les dés, les cartes ainsi que le pseudo d’un joueur
void init_tabCarte(DefCarte tabCarte[10]); //procédure qui va initialiser le tableau de définitions des cartes
void init_pioche(TPioche * pioche,TPioche * defausse); // Initialisation de la pioche.
void init_de(TJoueur leJoueur);// Procédure qui va donner 4 dés à un joueur
void nouveau_de(TJoueur joueur); // Procédure qui ajoute un dé à un joueur
TJoueur nouveauJoueur(int numJoueur, TPioche * pioche); //Procédure qui ajoute un joueur à la liste des joueurs  
TJoueur debut_partie(TJoueur tabJoueur[3]); // Procédure où chaque joueur va lancer ses dés pour savoir qui est celui qui va débuter le jeu et connaître le sens du jeu

void afficher_joueur(TJoueur joueur); // Procédure qui affiche le nombre de dés ainsi que le nombre de cartes d’un joueur
void afficher_les_joueurs(TJoueur tabJoueur[]);//procédure qui afficher tous les joueurs à l'écran
void afficher_lancer(TJoueur leJoueur);// procédure qui affiche les dés d'un joueur
void afficher_carte(TCarte * laCarte, DefCarte tabCarte[10]); //procédure qui affiche une carte

void piocher_carte(TJoueur leJoueur, TPioche * pioche); //Procédure qui va prendre un numéro dans la pioche, créer la carte et la mettre dans le deck des joueurs
void defausser_carte(TCarte * laCarte, TJoueur leJoueur, TPioche * defausse);// procédure qui va mettre une carte de la main d'un joueur dans la defausse 
void melanger_carte(TPioche * laPioche, TPioche * laDefausse);//prend les cartes de la défausse pour les mettre dans la pioche de façon aléatoire
int nombre_carte(TJoueur leJoueur); //Fonction qui retourne le nombre de dés du joueur passé en paramètre.
bool carte_utilisable(TCarte * laCarte, TJoueur joueur); // Fonction qui va, selon l’état actuel de la partie, déterminer si la carte peut être utilisée en fonction du nb de wazabi. Renvoie vrai si elle peut être utilisé, faux sinon

void lancer_des(TJoueur joueur); // Procédure qui va changer la valeur des dés d’un joueur, 1 pour le dé  “donner dé” , 2 pour le dé “piocher carte”, 3 pour le dé “Wasabi”
void echange_de(TJoueur tabJoueur[]);// Procédure qui va demander au joueur dans quel sens tourner les dés et ensuite faire l’échange des dés entre les joueurs
void egaliser_de(TJoueur leJoueur,int nbDeDeb,int nbDeNouveau);//Procédure qui va faire en sorte que le joueur ai un certains nombre de dé
int nombre_des(TJoueur leJoueur); // Fonction qui retourne le nombre de dés du joueur passé en paramètre.
void donner_de (TJoueur joueur1, TJoueur joueur2); // Procédure qui prend un dé du joueur1 pour le donner au joueur2
void supprimer_de(TJoueur joueur); // Procédure qui supprime le dé d'un joueur
int nb_wazabi(TJoueur joueur); //Fonction qui retourne le nombre de wazabi d'un joueur

TJoueur saisir_joueur(TJoueur joueurActuelle, TJoueur tabJoueur[3]);//Fonction qui va demander au joueur actuel, quel joueur séléctionner
void tour_suivant( TJoueur leJoueur,bool sens, TJoueur tabCarte[]); //procédure qui va changer le joueur actuel
int nombre_aleatoire(int min, int max);
int saisir_entre(int min, int max); 

// **********************
//  programme principal
// **********************
int main ()
{
    TJoueur tabJoueur[3];
    TPioche * pioche = NULL;  
    TPioche * defausse = NULL; 
     
    DefCarte tabCarte[10];
    bool sensAntiHoraire = true;

    init_tabCarte(tabCarte);

    init_partie(tabJoueur,pioche,defausse);

    return 0;
}
// ***************************************************************************************************************************************************************
//  Fonctions/Procédures début de la partie
// ***************************************************************************************************************************************************************

// Procédure qui initialise les dés, les cartes ainsi que le pseudo d’un joueur
void init_partie(TJoueur tabJoueur[3], TPioche * pioche, TPioche * defausse){
    int i;
    printf("******************************* Bienvenue dans le jeu Wazabi *************************************\n\n");
    init_pioche(pioche,defausse);
    for(i = 0; i < 3; i++)
    {
        tabJoueur[i] = nouveauJoueur(i,pioche);
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
    tabCarte[1].nbWasabi = 2;
    tabCarte[2].nbWasabi = 3;
    tabCarte[3].nbWasabi = 3;
    tabCarte[4].nbWasabi = 1;
    tabCarte[5].nbWasabi = 1;
    tabCarte[6].nbWasabi = 1;
    tabCarte[7].nbWasabi = 2;
    tabCarte[8].nbWasabi = 0;
    tabCarte[9].nbWasabi = 0;
}

// Initialisation de la pioche.
void init_pioche(TPioche * pioche, TPioche * defausse){
    int tabPioche[36] = {1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 9, 10, 10};

    TCarte * newCell;
    TCarte * aux;
    int i;


    for(i = 0; i < 35; i++)
    {
        newCell = (TCarte*) malloc (sizeof(TCarte));
        (*newCell).identifiant = tabPioche[i];
        (*newCell).carteSuivante = NULL;

        if((*defausse).sommet == NULL)
        { // La pioche est vide, on ajoute le premier élément.
            printf("\nici\n");
            (*defausse).sommet = newCell;
            
            aux = (*defausse).sommet;
        }
        else 
        {
            // La liste n'est pas vide, ajout après la dernière cellule.
            (*aux).carteSuivante = newCell;
            aux = newCell;
        }

    }
    

    melanger_carte(pioche, defausse);

}

// Procédure qui va donner 4 dés à un joueur
void init_de(TJoueur leJoueur){
    for(int i =0; i<4;i++){
        nouveau_de(leJoueur);
    }
}

// Procédure qui ajoute un dé à un joueur
void nouveau_de(TJoueur joueur){
    TDe * aux;
    TDe * newDe;
    do {
        aux = (*aux).deSuivant;
    } while((*aux).deSuivant != NULL);
    if((*aux).deSuivant == NULL)
    {
        (*newDe).valeur = -1;
        (*newDe).deSuivant = NULL;
        (*aux).deSuivant = newDe;
    }
}

//Procédure qui ajoute un joueur à la partie
TJoueur nouveauJoueur(int numJoueur, TPioche * pioche){ 
    TJoueur joueur;
    joueur.id = numJoueur;
    printf("Joueur n°%d veuillez saisir votre pseudo ? \n", numJoueur + 1);
    scanf(" %s", joueur.pseudo);
    joueur.joue = false;
    for(int i = 0; i < 4; i++)//initialisation des cartes du joueur
    {
        piocher_carte(joueur,pioche);
    }

    init_de(joueur);

    return joueur;
}
// Procédure où chaque joueur va lancer ses dés pour savoir qui est celui qui va débuter le jeu et connaître le sens du jeu
TJoueur debut_partie(TJoueur tabJoueur[3])
{
    int nbWazabi;
    int nbWazabiMax = 0;
    TJoueur joueurActuelle;

    for(int i = 0; i < 3; i++)
    {
        lancer_des(tabJoueur[i]);
        nbWazabi = nb_wazabi(tabJoueur[i]);

        if(nbWazabi > nbWazabiMax)
        {
            nbWazabiMax = nbWazabi;
            joueurActuelle = tabJoueur[i];
        }
    }  
    return joueurActuelle;
}

// ***************************************************************************************************************************************************************
//  Procédures affichage
// ***************************************************************************************************************************************************************

// Procédure qui affiche le nombre de dés ainsi que le nombre de cartes d’un joueur
void afficher_joueur(TJoueur joueur){
    int i;
    printf("Joueur n° %d \nPseudo: ", joueur.id);
    for(i = 0; i<24; i++){
        printf("%s",joueur.pseudo[i]);
    }
    int nbCartes = nombre_carte(joueur);
    int nbDes = nombre_des(joueur);
    printf("\nNombre de cartes: %d \n", nbCartes);
    printf("\nNombre de dés: %d \n", nbDes);
}

//procédure qui afficher tous les joueurs à l'écran
void afficher_les_joueurs(TJoueur tabJoueur[]){
    afficher_joueur(tabJoueur[0]);
    afficher_joueur(tabJoueur[1]);
    afficher_joueur(tabJoueur[2]);
}

// procédure qui affiche les dés d'un joueur
void afficher_lancer(TJoueur leJoueur){
    TDe * aux;
    aux = (TDe*) malloc(sizeof(TDe));
    int valeurDe;

    printf("Voici vos dés : ");

    aux = leJoueur.des;
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


//procédure qui affiche une carte
void afficher_carte(TCarte * laCarte, DefCarte tabCarte[10]){
    printf("Nombre de wasabi : %d : %s ",tabCarte[(*laCarte).identifiant].nbWasabi,tabCarte[(*laCarte).identifiant].libelle);
}


// ***************************************************************************************************************************************************************
// Procédure / fonctions cartes
// ***************************************************************************************************************************************************************

//Procédure qui va prendre un numéro dans la pioche, créer la carte et la mettre dans le deck des joueurs
void piocher_carte(TJoueur leJoueur, TPioche * pioche){
    TCarte * newCarte;
    
    newCarte = (*pioche).sommet;
    (*pioche).sommet = (*newCarte).carteSuivante;
    
    TCarte * aux;

    
    (*newCarte).carteSuivante=NULL;


    if(leJoueur.cartes==NULL){
        leJoueur.cartes = newCarte;
    }else{
        aux= leJoueur.cartes;
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

// procédure qui va mettre une carte de la main d'un joueur dans la defausse 
void defausser_carte(TCarte * laCarte, TJoueur leJoueur, TPioche * defausse){
    bool supprimer = false;
    bool premier = true;
    TCarte * aux;
    TCarte * prec;

    aux = leJoueur.cartes;
    prec = aux;
    while (aux != NULL && !supprimer){
        if((*aux).identifiant == (*laCarte).identifiant){
            if(premier){
                leJoueur.cartes=(*aux).carteSuivante; 
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

//prend les cartes de la défausse pour les mettre dans la pioche de façon aléatoire
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
        parcours = 1;
        valeur = nombre_aleatoire(1,nbCarteDef);
        while(parcours != valeur){
            prec = aux;
            aux = (*aux).carteSuivante;
            parcours = parcours +1;
        }

        if(valeur == 1){
            (*laDefausse).sommet=(*aux).carteSuivante;
        }else{
            (*prec).carteSuivante=(*aux).carteSuivante;
        }

        if((*laPioche).sommet==NULL){
            (*laPioche).sommet = aux;
            (*aux).carteSuivante = NULL;
        }else{
            (*aux).carteSuivante = (*laPioche).sommet;
            (*laPioche).sommet = aux;
        }
    }

}

//Fonction qui retourne le nombre de dés du joueur passé en paramètre.
int nombre_carte(TJoueur leJoueur){
    int nbCarte; //Nombre de carte 
    TCarte * aux;
    leJoueur.cartes = aux;
    nbCarte = 0;
    if(aux == NULL)
    {
        return nbCarte;
    }
    else
    {
        while(aux != NULL)
        {
            nbCarte = nbCarte + 1;
            aux = (*aux).carteSuivante;
        }
    }
    return nbCarte;
}
// Fonction qui va, selon l’état actuel de la partie, déterminer si la carte peut être utilisée en fonction du nb de wazabi. Renvoie vrai si elle peut être utilisé, faux sinon
bool carte_utilisable(DefCarte laCarte, TJoueur joueur)
{
    bool utilisable = false;
    int nbWazabiJoueur;

    nbWazabiJoueur = nb_wazabi(joueur);    
    if(laCarte.nbWasabi <= nbWazabiJoueur)
    {
        utilisable = true;
    }
    return utilisable;
}

// ***************************************************************************************************************************************************************
// Procédure / fonctions dés
// ***************************************************************************************************************************************************************

// Procédure qui va changer la valeur des dés d’un joueur, 1 pour le dé  “donner dé” , 2 pour le dé “piocher carte”, 3 pour le dé “Wasabi”
void lancer_des(TJoueur joueur){
    TDe * aux;
    aux = joueur.des;

    do {
        (*aux).valeur = nombre_aleatoire(1,3);
        aux = (*aux).deSuivant;
    } while(aux != NULL);
}

// Procédure qui va demander au joueur dans quel sens tourner les dés et ensuite faire l’échange des dés entre les joueurs
void echange_de(TJoueur tabJoueur[]){

    printf("Dans quelle sens voulez vous tournez les dés ? (0 pour sens horaire, 1 pour sens anti horaire)\n ");
    int sens = saisir_entre(0,1);
    int de1 = nombre_des(tabJoueur[0]);
    int de2 = nombre_des(tabJoueur[1]);
    int de3 = nombre_des(tabJoueur[2]);

    if(sens){
        egaliser_de(tabJoueur[0],de1,de3);
        egaliser_de(tabJoueur[1],de2,de1);
        egaliser_de(tabJoueur[2],de3,de2);
    }else{
        egaliser_de(tabJoueur[0],de1,de2);
        egaliser_de(tabJoueur[1],de2,de3);
        egaliser_de(tabJoueur[2],de3,de1);    
    }
}

// Procédure qui prend un dé du joueur1 pour le donner au joueur2
void donner_de (TJoueur joueur1, TJoueur joueur2){
    supprimer_de(joueur1);
    nouveau_de(joueur2);
}

//Procédure qui va faire en sorte que le joueur ai un certains nombre de dé
void egaliser_de(TJoueur leJoueur,int nbDeDeb,int nbDeNouveau){
    int nbDeAct = nbDeDeb;
    while (nbDeAct != nbDeNouveau){
        if(nbDeAct>nbDeNouveau){
            //supprimer_de(leJoueur);
            nbDeAct = nbDeAct -1;
        }else{
            nouveau_de(leJoueur);
            nbDeAct = nbDeAct +1;
        }
    }
}

// Fonction qui retourne le nombre de dés du joueur passé en paramètre.
int nombre_des(TJoueur leJoueur){
    TDe * aux;
    leJoueur.des = aux;
    int nbDes = 0;
    if(aux == NULL)
    {
        return nbDes;
    }
    else
    {
        while(aux != NULL)
        {
            nbDes = nbDes + 1;
            aux = aux->deSuivant;
        }
    }
    return nbDes;
}
// Procédure qui supprime le dé d'un joueur
void supprimer_de(TJoueur joueur)
{
    TDe * aux;

    aux = joueur.des;
    joueur.des = (*aux).deSuivant;

    free(aux);
}
//Fonction qui retourne le nombre de wazabi d'un joueur
int nb_wazabi(TJoueur joueur)
{
    TDe * aux;
    int nbWazabi = 0;

    aux = joueur.des;

    while (aux != NULL)
    {
        if((*aux).valeur == 3)
        {
            nbWazabi = nbWazabi + 1;
        }
        aux = (*aux).deSuivant;
    }
    return nbWazabi;
} 
// ***************************************************************************************************************************************************************
// Procédure / fonctions déroulement de la partie
// ***************************************************************************************************************************************************************

//Fonction qui va demander au joueur actuel, quel joueur séléctionner
TJoueur saisir_joueur(TJoueur joueurActuelle, TJoueur tabJoueur[3])
{
    TJoueur joueurSelect;
    TJoueur joueur1;
    TJoueur joueur2;
    int nb;

    if(joueurActuelle.id == 0)
    {
        joueur1 = tabJoueur[1];
        joueur2 = tabJoueur[2];
    }
    else if(joueurActuelle.id == 1)
    {
        joueur1 = tabJoueur[0];
        joueur2 = tabJoueur[2];
    }
    else
    {
        joueur1 = tabJoueur[0];
        joueur2 = tabJoueur[1];
    }
    do
    {
        printf("Veuillez saisir un joueur (autre que vous même) \n");
        printf("1 - "); 
        //afficher_joueur(joueur1);
        printf("2 - "); 
        //afficher_joueur(joueur2);
        scanf(" %d", nb);
    } while (nb > 2 || nb < 1);
   
    switch(nb)
    {
        case 1:
            joueurSelect = joueur1;
        break;
        case 2:
            joueurSelect = joueur2;
        break;
    }

    return joueurSelect;    
}

//procédure qui va changer le joueur actuel
void tour_suivant(TJoueur leJoueur,bool sens, TJoueur tabJoueur[]){
    leJoueur.joue = false;

    if(sens){
        if(leJoueur.id == 1){
            leJoueur=tabJoueur[2];
        }else{
            if(leJoueur.id == 2){
                leJoueur = tabJoueur[3];
            }else{
                if(leJoueur.id == 3){
                    leJoueur = tabJoueur[1];
                }
            }
        }     
    }else{
        if(leJoueur.id == 1){
            leJoueur=tabJoueur[3];
        }else{
            if(leJoueur.id == 2){
                leJoueur = tabJoueur[1];
            }else{
                if(leJoueur.id == 3){
                    leJoueur = tabJoueur[2];
                }
            }
        }   
    }

    leJoueur.joue=true;
}

// fonction qui retourne un nombre aléatoire compris entre deux nombres
int nombre_aleatoire(int min, int max){
    int nbMystere;

    nbMystere = (rand() % (max-min - 1)) + min;

    return nbMystere;
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