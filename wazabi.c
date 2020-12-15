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
    bool joue;	//determine si c'est le tour du joueur
    int nbCartePrison; //0 si il peut joueur, 1 si il doit passer un tour, 2 si il doit passer 2 tours
} TJoueur;



// **********************
//  prototypes des fonctions
// **********************
void init_partie(TJoueur tabJoueur[], TPioche * pioche, TPioche * defausse); // Procédure qui initialise les dés, les cartes ainsi que le pseudo d’un joueur
void init_tabCarte(DefCarte tabCarte[10]); //procédure qui va initialiser le tableau de définitions des cartes
void init_pioche(TPioche * pioche,TPioche * defausse); // Initialisation de la pioche.
void init_de(TJoueur * leJoueur);// Procédure qui va donner 4 dés à un joueur
void nouveau_de(TJoueur * joueur); // Procédure qui ajoute un dé à un joueur
TJoueur nouveauJoueur(int numJoueur, TPioche * pioche, TPioche * defausse); //Procédure qui ajoute un joueur à la liste des joueurs  
TJoueur debut_partie(TJoueur tabJoueur[3]); // Procédure où chaque joueur va lancer ses dés pour savoir qui est celui qui va débuter le jeu et connaître le sens du jeu

void afficher_joueur(TJoueur joueur); // Procédure qui affiche le nombre de dés ainsi que le nombre de cartes d’un joueur
void afficher_les_joueurs(TJoueur tabJoueur[]);//procédure qui afficher tous les joueurs à l'écran
void afficher_lancer(TJoueur leJoueur);// procédure qui affiche les dés d'un joueur
void afficher_carte(int idCarte, DefCarte tabCarte[10]); //procédure qui affiche une carte
void afficher_cartes_joueur(TJoueur * leJoueur,DefCarte tabCarte[10]);// Procédure qui affiche les cartes du joueur passé en paramètre
int afficher_cartes_utilisables(TJoueur * leJoueur, DefCarte tabCarte[10], TJoueur tabJoueur[]); // Procédure qui affiche les cartes utilisables par le joueur passé en paramètre

void piocher_carte(TJoueur * leJoueur, TPioche * pioche,TPioche * defausse); //Procédure qui va prendre un numéro dans la pioche, créer la carte et la mettre dans le deck des joueurs
void defausser_carte(int idCarte, TJoueur * leJoueur, TPioche * defausse);// procédure qui va mettre une carte de la main d'un joueur dans la defausse 
void melanger_carte(TPioche * laPioche, TPioche * laDefausse);//prend les cartes de la défausse pour les mettre dans la pioche de façon aléatoire
int nombre_carte(TJoueur * leJoueur); //Fonction qui retourne le nombre de dés du joueur passé en paramètre.
bool carte_utilisable(DefCarte tabCarte[], int idCarte,TJoueur tabJoueur[], TJoueur joueurActuel); // Fonction qui va, selon l’état actuel de la partie, déterminer si la carte peut être utilisée en fonction du nb de wazabi. Renvoie vrai si elle peut être utilisé, faux sinon
void utiliser_carte(TJoueur * joueurActuel, int carte, TJoueur tabJoueur[],TPioche * pioche, TPioche * defausse); // Procédure permettant de jouer une carte parmis le deck d’un joueur et d’utiliser son effet 
void choix_carteUtilisables(TJoueur * joueur, DefCarte tabCarte[10], TJoueur tabJoueur[],TPioche * defausse,TPioche * pioche,int * carteDefausser); // Procédure qui va demander au joueur quelle carte choisir parmis ses cartes utilisables

void lancer_des(TJoueur joueur); // Procédure qui va changer la valeur des dés d’un joueur, 1 pour le dé  “donner dé” , 2 pour le dé “piocher carte”, 3 pour le dé “Wasabi”
void echange_de(TJoueur tabJoueur[]);// Procédure qui va demander au joueur dans quel sens tourner les dés et ensuite faire l’échange des dés entre les joueurs
void egaliser_de(TJoueur * leJoueur,int nbDeDeb,int nbDeNouveau);//Procédure qui va faire en sorte que le joueur ai un certains nombre de dé
int nombre_des(TJoueur leJoueur); // Fonction qui retourne le nombre de dés du joueur passé en paramètre.
void donner_de (TJoueur * joueur1, TJoueur * joueur2); // Procédure qui prend un dé du joueur1 pour le donner au joueur2
void supprimer_de(TJoueur * joueur); // Procédure qui supprime le dé d'un joueur

int nb_wazabi(TJoueur joueur); //Fonction qui retourne le nombre de wazabi d'un joueur
int nb_piocherCarte(TJoueur joueur) ; //Fonction qui retourne le nombre de dé piocher carte d'un joueur
int nb_donnerDe(TJoueur joueur); //Fonction qui retourne le nombre de donneDe d'un joueur 
int nb_occurrenceDe(TJoueur * joueur, int idDe); // Fonction qui retourne le nombre d'occurrence d'un dé selon son identifiant

TJoueur saisir_joueur(TJoueur joueurActuel, TJoueur tabJoueur[3]);//Fonction qui va demander au joueur actuel, quel joueur séléctionner
void tour_suivant( TJoueur * leJoueur,bool * sens, TJoueur tabJoueur[], DefCarte tabCarte[], TPioche * pioche, TPioche * defausse); //procédure qui va changer le joueur actuel
void rejouer_changement_sens();//procédure pour la carte qui fait rejouer le joueur et change le sens du jeu
int nombre_aleatoire(int min, int max);
int saisir_entre(int min, int max); 
int saisir_suivant(); //fonction qui demande à l'utilisateur de rentrer "1" pour passer a l'action suivante
void space(); //fonction qui va faire un \n

// **********************
//  programme principal
// **********************
int main ()
{
    TJoueur tabJoueur[3];
    TJoueur joueurActuel;
    TPioche * defausse; 
    TPioche * pioche;  
    defausse = (TPioche*) malloc (sizeof(TPioche)); 
    pioche = (TPioche*) malloc (sizeof(TPioche)); 
    DefCarte tabCarte[10];
    bool gagner = false;
    bool sensAntiHoraire = true;

    init_tabCarte(tabCarte);
    init_partie(tabJoueur,pioche,defausse);
    space();

    do{ 
        joueurActuel=debut_partie(tabJoueur);
    
    }while(!joueurActuel.joue);
    
    //supprimer_de(&joueurActuel);
    //donner_de(&tabJoueur[0],&tabJoueur[1]);
    do{
        tour_suivant(&joueurActuel, &sensAntiHoraire, tabJoueur, tabCarte, pioche, defausse);
        for(int i=0;i<3;i++){
            if(nombre_des(tabJoueur[i])==0){
                gagner = true;
            }
        }
    }while(!gagner);

    if(nombre_des(tabJoueur[0]))
    {
        joueurActuel=tabJoueur[0];
    }
    if(nombre_des(tabJoueur[1]))
    {
        joueurActuel=tabJoueur[1];
    }
    if(nombre_des(tabJoueur[2]))
    {
        joueurActuel=tabJoueur[2];
    }
    printf("%s a gagné la partie", joueurActuel.pseudo);

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
        tabJoueur[i] = nouveauJoueur(i,pioche,defausse);
    } 
    
}

void init_tabCarte(DefCarte tabCarte[10]){
    strcpy(tabCarte[0].libelle, "Supprimez 1 de vos des \n");
    strcpy(tabCarte[1].libelle, "Tous les joueurs donnent leurs des a leur voisin de gauche ou de droite \n");
    strcpy(tabCarte[2].libelle, "Supprimez 2 de vos des \n");
    strcpy(tabCarte[3].libelle, "Donnez 1 de vos des au joueur de votre choix \n");
    strcpy(tabCarte[4].libelle, "Prenez 1 carte au joueur de votre choix \n");
    strcpy(tabCarte[5].libelle, "Le joueur de votre choix n’a plus qu’1 carte \n");
    strcpy(tabCarte[6].libelle, "Piochez 3 cartes dans la pioche \n");
    strcpy(tabCarte[7].libelle, "Tous les joueurs sauf vous n’ont plus que 2 cartes\n");
    strcpy(tabCarte[8].libelle, "Le joueur de votre choix passe son tour\n");
    strcpy(tabCarte[9].libelle, "Rejouez et changez de sens \n");
  
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
void init_de(TJoueur * leJoueur){
    for(int i =0; i<4;i++)
    {
        nouveau_de(leJoueur);
    }
}

// Procédure qui ajoute un dé à un joueur
void nouveau_de(TJoueur * joueur){
    TDe * aux;
    TDe * newDe;
    newDe = (TDe*) malloc (sizeof(TDe));
    (*newDe).valeur = 1;
    (*newDe).deSuivant = NULL;

    aux = (*joueur).des;
    

    if(aux == NULL)
    {
    
        (*joueur).des = newDe;
    }else
    {
        while ((*aux).deSuivant !=  NULL)
        {
            aux=(*aux).deSuivant;
        }
        (*aux).deSuivant=newDe;
    }

}


//Procédure qui ajoute un joueur à la partie
TJoueur nouveauJoueur(int numJoueur, TPioche * pioche, TPioche * defausse){ 
    TJoueur joueur;
    joueur.cartes=NULL;
    joueur.des=NULL;
    joueur.id = numJoueur;
    joueur.nbCartePrison = 0;

    printf("Joueur n°%d veuillez saisir votre pseudo : \n", numJoueur + 1);
    scanf(" %s", joueur.pseudo);
    joueur.pseudo[24]= "\0";
   
    joueur.joue = false;
    for(int i = 0; i < 3; i++)//initialisation des cartes du joueur
    {
        piocher_carte(&joueur,pioche,defausse);
    }

    init_de(&joueur);


    return joueur;
}
// Procédure où chaque joueur va lancer ses dés pour savoir qui est celui qui va débuter le jeu et connaître le sens du jeu
TJoueur debut_partie(TJoueur tabJoueur[3])
{
    TJoueur joueurActuel;
    int nbWazabi=0;
    int nbWazabiMax = 0;
    bool egalite = false;

    for(int i = 0; i < 3; i++)
    {
        space();
        printf("Joueur %d : %s, veuillez lancer vos dés :\n", i+1, &tabJoueur[i].pseudo);
        saisir_suivant();
        
        lancer_des(tabJoueur[i]);
        nbWazabi=nb_wazabi(tabJoueur[i]);

        printf("Joueur %d : %s a obtenu %d wazabi(s)\n", i+1, &tabJoueur[i].pseudo,nbWazabi );
        saisir_suivant();
        

        space();

        if(nbWazabi >= nbWazabiMax)  
        {    
            joueurActuel = tabJoueur[i];
            if(nbWazabi == nbWazabiMax)
            {
                if(i==2)
                {
                    egalite=true;
                }else{
                    if(i==1)
                    {
                        egalite = true;
                    }
                }
            }else
            {
                nbWazabiMax=nbWazabi;
                if(i==2)
                {
                    egalite=false;
                }
            }
        }
        
    }  
    
    if(egalite)
    {
        printf("Egalité, veuillez recommencer\n");
    }else
    {
        joueurActuel.joue=true;
        printf("Le joueur %d : %s va debuter la partie\n", joueurActuel.id+1, joueurActuel.pseudo);
    }

    return joueurActuel;
}

// ***************************************************************************************************************************************************************
//  Procédures affichage
// ***************************************************************************************************************************************************************

// Procédure qui affiche le nombre de dés ainsi que le nombre de cartes d’un joueur
void afficher_joueur(TJoueur joueur){
    printf("Joueur n° %d Pseudo: %s\n", joueur.id+1, &joueur.pseudo);
    int nbCartes = nombre_carte(&joueur);
    int nbDes = nombre_des(joueur);
    printf("Nombre de cartes: %d \n", nbCartes);
    printf("Nombre de dés: %d \n", nbDes);
}

//procédure qui afficher tous les joueurs à l'écran
void afficher_les_joueurs(TJoueur tabJoueur[]){
    afficher_joueur(tabJoueur[0]);
    space();
    afficher_joueur(tabJoueur[1]);
    space();
    afficher_joueur(tabJoueur[2]);
}

// procédure qui affiche les dés d'un joueur
void afficher_lancer(TJoueur leJoueur){
    TDe * aux;
    //aux = (TDe*) malloc(sizeof(TDe));
    int valeurDe;

    printf("Voici vos dés : ");

    aux = leJoueur.des;
    while(aux != NULL){
        valeurDe = (*aux).valeur;
        switch (valeurDe)
        {
        case 1:
            printf("Donner dé");
            break;
        case 2:
            printf("Piocher carte");
            break;
        case 3:
            printf("Wazabi");
            break;
        default:
            break;
        }
        printf(" - ");
        aux=(*aux).deSuivant;
    }
}


//procédure qui affiche une carte
void afficher_carte(int idCarte, DefCarte tabCarte[10]){
    printf("Nombre de wazabi : %d, %s ",tabCarte[idCarte-1].nbWasabi,tabCarte[idCarte-1].libelle);
}

// Procédure qui affiche les cartes du joueur passé en paramètre
void afficher_cartes_joueur(TJoueur * leJoueur,DefCarte tabCarte[10]){
    TCarte * aux;
    aux = (*leJoueur).cartes ;
    int i = 1;
    while(aux != NULL)
    {              
        printf("Carte n°%d:\n", i,(*aux).identifiant );
        afficher_carte((*aux).identifiant, tabCarte);
        i=i+1;    
        aux = (*aux).carteSuivante;
    }
}


// Procédure qui affiche les cartes utilisables par le joueur passé en paramètre
int afficher_cartes_utilisables(TJoueur * leJoueur, DefCarte tabCarte[10], TJoueur tabJoueur[]){
    TCarte * aux;
    aux = (*leJoueur).cartes ;
    int i =1;
    space();
    printf("Voici les cartes que vous pouvez utiliser :\n");
    while(aux != NULL)
    {              
        if(carte_utilisable(tabCarte, (*aux).identifiant, tabJoueur, (*leJoueur)))
        {
            printf("Carte n°%d:\n",i);
            afficher_carte((*aux).identifiant, tabCarte);
            i=i+1;
        }
        aux = (*aux).carteSuivante;
    }
    return i;
}

// ***************************************************************************************************************************************************************
// Procédure / fonctions cartes
// ***************************************************************************************************************************************************************

//Procédure qui va prendre un numéro dans la pioche, créer la carte et la mettre dans le deck des joueurs
void piocher_carte(TJoueur * leJoueur, TPioche * pioche, TPioche * defausse){
    
    if((*pioche).sommet==NULL)
    {
        melanger_carte(pioche,defausse);
    }
    

    TCarte * newCarte;
    newCarte = (*pioche).sommet;
    TCarte * aux;
    aux = (*pioche).sommet;
    
    
    (*pioche).sommet = (*(*pioche).sommet).carteSuivante;
    
    
    (*newCarte).carteSuivante = NULL;



    if((*leJoueur).cartes==NULL)
    {
        (*leJoueur).cartes = newCarte;
    }else
    {
        aux= (*leJoueur).cartes;


        while ((*aux).carteSuivante!=NULL)
        {
            aux =(*aux).carteSuivante;
        }
        (*aux).carteSuivante = newCarte;
    }
}

// procédure qui va mettre une carte de la main d'un joueur dans la defausse 
void defausser_carte(int idCarte, TJoueur * leJoueur, TPioche * defausse){
    bool supprimer = false;
    bool premier = true;
    TCarte * aux;
    TCarte * prec;

    aux = (*leJoueur).cartes;
    prec = aux;
    while (aux != NULL && !supprimer)
    {
        if((*aux).identifiant == idCarte)
        {
            if(premier)
            {
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

//prend les cartes de la défausse pour les mettre dans la pioche de façon aléatoire
void melanger_carte(TPioche * laPioche, TPioche * laDefausse){
    int valeur = 0;
    int nbCarteDef = 0;
    int parcours =1;
    
    TCarte * aux;
    TCarte * prec;

    aux = (*laDefausse).sommet;

    while(aux != NULL)
    {
        nbCarteDef = nbCarteDef+1;
        aux = (*aux).carteSuivante;
    }

    while((*laDefausse).sommet != NULL)
    {
        aux = (*laDefausse).sommet;
        parcours = 1;
        if(nbCarteDef!=1)
        {
            valeur = nombre_aleatoire(1,nbCarteDef);
        }else{
            valeur = 1;
        }

        while(parcours != valeur)
        {
            prec = aux;
            aux = (*aux).carteSuivante;
            parcours = parcours +1;
        }

        if(valeur == 1)
        {
            if((*aux).carteSuivante != NULL)
            {
                (*laDefausse).sommet=(*aux).carteSuivante;
            }else
            {
                (*laDefausse).sommet=NULL;
            }

        }else
        {
            if((*aux).carteSuivante != NULL)
            {
                (*prec).carteSuivante=(*aux).carteSuivante;
            }else
            {
                (*prec).carteSuivante=NULL;
            }
        }

        if((*laPioche).sommet==NULL){
            (*laPioche).sommet = aux;
            (*aux).carteSuivante = NULL;
        }else{
            (*aux).carteSuivante = (*laPioche).sommet;
            (*laPioche).sommet = aux;
        }

        nbCarteDef = nbCarteDef-1;

    }

}
//Fonction qui retourne le nombre de dés du joueur passé en paramètre.
int nombre_carte(TJoueur * leJoueur){
    int nbCarte; //Nombre de carte 
    TCarte * aux;
    aux= (*leJoueur).cartes ;
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
bool carte_utilisable(DefCarte tabCarte[], int idCarte,TJoueur tabJoueur[], TJoueur joueurActuel)
{
    bool utilisable = false;
    int nbWazabiJoueur;

    nbWazabiJoueur = nb_wazabi(joueurActuel);    
    if(tabCarte[idCarte-1].nbWasabi <= nbWazabiJoueur)
    {
        int nbDonneDe = nb_donnerDe(joueurActuel);
        if(idCarte == 0 && nbDonneDe < 2)
        {
            utilisable = true;
        }
        else if(idCarte == 5)
        {
            //Test combien de joueur on 2 cartes ou plus
            int nbCarte2OuPlus = 0;
            for(int i = 0; i < 3; i++)
            {
                int nbCarteJoueur = nombre_carte(&tabJoueur[i]);
                if(nbCarteJoueur >= 2)
                {
                    nbCarte2OuPlus = nbCarte2OuPlus + 1;
                }         
            } 
            int nbCarteJoueurActuel = nombre_carte(&joueurActuel);
            if(nbCarte2OuPlus > 1)
            {
                utilisable = true;
            } 
            else if(nbCarte2OuPlus == 1 && nbCarteJoueurActuel < 2)
            {
                utilisable = true;
            }  
        }     
        else if(idCarte == 7)
        {
            //Test combien de joueur on 3 cartes ou plus
            int nbCarte3OuPlus = 0;
            for(int i = 0; i < 3; i++)
            {
                int nbCarteJoueur = nombre_carte(&tabJoueur[i]);
                if(nbCarteJoueur >= 3)
                {
                    nbCarte3OuPlus = nbCarte3OuPlus + 1;
                }         
            } 
            int nbCarteJoueurActuel = nombre_carte(&joueurActuel);
            if(nbCarte3OuPlus > 1)
            {
                utilisable = true;
            } 
            else if(nbCarte3OuPlus == 1 && nbCarteJoueurActuel < 3)
            {
                utilisable = true;
            }  
        }else{
            utilisable=true;
        }
    }
    return utilisable;
}

// Procédure permettant de jouer une carte parmis le deck d’un joueur et d’utiliser son effet 
void utiliser_carte(TJoueur * joueurActuel, int carte, TJoueur tabJoueur[],TPioche * pioche, TPioche * defausse)
{
    TJoueur * joueur;
    joueur = (TJoueur*) malloc (sizeof(TJoueur)); 

    switch(carte)
    {
        case 1: 
            supprimer_de(joueurActuel);
            tabJoueur[(*joueurActuel).id]=(*joueurActuel);
        break;
        case 2:
            echange_de(tabJoueur);
        break;
        case 3:
            supprimer_de(joueurActuel);
            supprimer_de(joueurActuel);
            tabJoueur[(*joueurActuel).id]=(*joueurActuel);
        break;
        case 4:
            (*joueur) = saisir_joueur((*joueurActuel),tabJoueur);
            donner_de(joueurActuel,joueur);   
            tabJoueur[(*joueurActuel).id]=(*joueurActuel);
            tabJoueur[(*joueur).id]=(*joueur); 
        break;
        case 5:
            //piocher_carte_deck()
        break;
        case 6:
            //reste_1_carte()
        break;
        case 7:
            piocher_carte(joueurActuel,pioche,defausse);
            piocher_carte(joueurActuel,pioche,defausse);
            piocher_carte(joueurActuel,pioche,defausse);
            tabJoueur[(*joueurActuel).id]=(*joueurActuel);
        break;
        case 8:
            //reste_2_cartes()
        break;
        case 9:
            (*joueur) = saisir_joueur((*joueurActuel),tabJoueur);
            (*joueur).nbCartePrison = (*joueur).nbCartePrison + 1;
            tabJoueur[(*joueur).id]=(*joueur); 
        break;
        case 10:
            rejouer_changement_sens();
        break;
        
        free(joueur);
    }
    //defausser_carte(carte,&joueurActuel,defausse);
}

// Procédure qui va demander au joueur quelle carte choisir parmis ses cartes utilisables
void choix_carteUtilisables(TJoueur * joueur, DefCarte tabCarte[10], TJoueur tabJoueur[], TPioche * defausse,TPioche * pioche,int * carteDefausser){
    int nbCarte = afficher_cartes_utilisables(joueur, tabCarte, tabJoueur);
    
    printf("Quelle carte souhaitez-vous utiliser ? (0 pour ne pas en utiliser)\n");
    
    int laCarteChoisi = saisir_entre(0,nbCarte-1);
    int i = 1;
    bool defausser = false;
    int idCarte=0;

    TCarte * aux;
    if(laCarteChoisi!=0){
        aux = (*joueur).cartes ;
        while(aux != NULL && !defausser)
        {              
            if(carte_utilisable(tabCarte, (*aux).identifiant, tabJoueur, (*joueur)))
            {
                if(laCarteChoisi==i)
                {
                    idCarte=(*aux).identifiant;
                    defausser_carte((*aux).identifiant,joueur,defausse);
                    defausser=true;
                }
                i=i+1;
            }
            aux = (*aux).carteSuivante;
        }
        
        utiliser_carte(joueur,idCarte,tabJoueur,pioche,defausse);
        tabJoueur[(*joueur).id]=(*joueur);
    }
        

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

    printf("Dans quel sens voulez-vous tourner les dés ? (1 pour sens horaire, 0 pour sens anti-horaire)\n ");
    int sens = saisir_entre(0,1);
    int de1 = nombre_des(tabJoueur[0]);
    int de2 = nombre_des(tabJoueur[1]);
    int de3 = nombre_des(tabJoueur[2]);

    if(sens){
        egaliser_de(&tabJoueur[0],de1,de3);
        egaliser_de(&tabJoueur[1],de2,de1);
        egaliser_de(&tabJoueur[2],de3,de2);
    }else
    {
        egaliser_de(&tabJoueur[0],de1,de2);
        egaliser_de(&tabJoueur[1],de2,de3);
        egaliser_de(&tabJoueur[2],de3,de1);    
    }
}

// Procédure qui prend un dé du joueur1 pour le donner au joueur2
void donner_de (TJoueur * joueur1, TJoueur * joueur2){
    supprimer_de(joueur1);
    nouveau_de(joueur2);
}

//Procédure qui va faire en sorte que le joueur ai un certains nombre de dé
void egaliser_de(TJoueur * leJoueur,int nbDeDeb,int nbDeNouveau){
    int nbDeAct = nbDeDeb;
    while (nbDeAct != nbDeNouveau){
        if(nbDeAct>nbDeNouveau)
        {
            supprimer_de(leJoueur);
            nbDeAct = nbDeAct -1;
        }else
        {
            nouveau_de(leJoueur);
            nbDeAct = nbDeAct +1;
        }
    }

}

// Fonction qui retourne le nombre de dés du joueur passé en paramètre.
int nombre_des(TJoueur leJoueur){

    TDe * aux;
    aux = leJoueur.des;
    int nbDes = 0;

    if(aux != NULL)
    { 
        while(aux != NULL)
        {
            nbDes = nbDes + 1;
            aux = (*aux).deSuivant;
        }
    }

    return nbDes;
}

// Procédure qui supprime le dé d'un joueur
void supprimer_de(TJoueur * joueur)
{
    TDe * aux;
    

    aux = (*joueur).des;
    (*joueur).des = (*aux).deSuivant;
    (*aux).deSuivant = NULL;

    free(aux);
}

//Fonction qui retourne le nombre de wazabi d'un joueur
int nb_wazabi(TJoueur joueur)
{
    int nbWazabi = 0;

    nbWazabi = nb_occurrenceDe(&joueur,3);

    return nbWazabi;
} 

//Fonction qui retourne le nombre de dé piocher carte d'un joueur 
int nb_piocherCarte(TJoueur joueur)
{
    int nbPiocherCarte = 0;

    nbPiocherCarte = nb_occurrenceDe(&joueur,2);

    return nbPiocherCarte;
}

//Fonction qui retourne le nombre de donneDe d'un joueur 
int nb_donnerDe(TJoueur joueur)
{
    int nbDonneDe = 0;

    nbDonneDe = nb_occurrenceDe(&joueur,1);
    
    return nbDonneDe;
}


// Fonction qui retourne le nombre d'occurrence d'un dé selon son identifiant
int nb_occurrenceDe(TJoueur * joueur, int idDe)
{
    TDe * aux;
    int nbOccurrence = 0;

    aux = (*joueur).des;

    while (aux != NULL)
    {
        if((*aux).valeur == idDe)
        {
            nbOccurrence = nbOccurrence + 1;
        }
        aux = (*aux).deSuivant;
    }
    return nbOccurrence;
}

// ***************************************************************************************************************************************************************
// Procédure / fonctions déroulement de la partie
// ***************************************************************************************************************************************************************

//Fonction qui va demander au joueur actuel, quel joueur séléctionner
TJoueur saisir_joueur(TJoueur joueurActuel, TJoueur tabJoueur[3])
{
    TJoueur joueurSelect;
    TJoueur joueur1;
    TJoueur joueur2;
    int nb;

    if(joueurActuel.id == 0)
    {
        joueur1 = tabJoueur[1];
        joueur2 = tabJoueur[2];
    }
    else if(joueurActuel.id == 1)
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
        afficher_joueur(joueur1);
        printf("2 - "); 
        afficher_joueur(joueur2);
        scanf("%d", &nb);
    } while (nb > 2 || nb < 1);

    switch(nb)
    {
        case 1:
            joueurSelect = joueur1;
        break;
        case 2:
            joueurSelect = joueur2;
        break;
        default:
            break;
    }
    return joueurSelect;    
}

//procédure du deroulement d'un tour
void tour_suivant(TJoueur * leJoueur,bool * sens, TJoueur tabJoueur[], DefCarte tabCarte[], TPioche * pioche, TPioche * defausse){
    printf("==================================== Tour du joueur %d: %s ====================================\n\n", (*leJoueur).id+1, (*leJoueur).pseudo);
    int idCarte;
    afficher_les_joueurs(tabJoueur);
    space();

    if((*leJoueur).nbCartePrison==0)
    {
        int nbDonnerDe=0;
        int nbPiocherCarte=0;
        int nbWasabi=0;
        TJoueur * cible;
        cible = (TJoueur*) malloc (sizeof(TJoueur)); 

        printf("Veuillez lancer vos dés\n");
        saisir_suivant();
        lancer_des((*leJoueur));
        nbDonnerDe = nb_donnerDe((*leJoueur));
        nbPiocherCarte = nb_piocherCarte((*leJoueur));
        nbWasabi = nb_wazabi((*leJoueur));
        afficher_lancer((*leJoueur));
        saisir_suivant();

        if(nbDonnerDe>0)
        {
            for(int i=0;i<nbDonnerDe;i++)
            {
                printf("A qui donner le dé ?\n");
                (*cible) = saisir_joueur((*leJoueur),tabJoueur);
                donner_de(leJoueur,cible);
                tabJoueur[(*leJoueur).id]=(*leJoueur);
                tabJoueur[(*cible).id]=(*cible);
            }
        }
        
        if(nbPiocherCarte>0)
        {
            for(int i=0;i<nbPiocherCarte;i++)
            {
                piocher_carte(leJoueur,pioche,defausse);
            }
        }
        free(cible);

        space();
        printf("Vos cartes :\n\n");
        afficher_cartes_joueur((leJoueur),tabCarte);

        choix_carteUtilisables(leJoueur,tabCarte,tabJoueur,defausse,pioche,&idCarte);
        tabJoueur[(*leJoueur).id]=(*leJoueur);
        
    }else
    {
        space();
        printf("Vos cartes :\n\n");
        afficher_cartes_joueur((leJoueur),tabCarte);
        printf("Vous passez votre tour ! \n");
        (*leJoueur).nbCartePrison = (*leJoueur).nbCartePrison-1;
    }

    if(idCarte!=10)
    {
        // determine le joueur suivant
        (*leJoueur).joue = false;

        if(sens)
        {
            if((*leJoueur).id == 0)
            {
                (*leJoueur)= tabJoueur[1];
            }else
            {
                if((*leJoueur).id == 1)
                {
                    (*leJoueur) = tabJoueur[2];
                }else
                {
                    if((*leJoueur).id == 2)
                    {
                        (*leJoueur) = tabJoueur[0];
                    }
                }
            }     
        }else
        {
            if((*leJoueur).id == 0)
            {
                (*leJoueur)=tabJoueur[2];
            }else
            {
                if((*leJoueur).id == 1)
                {
                    (*leJoueur) = tabJoueur[0];
                }else
                {
                    if((*leJoueur).id == 2)
                    {
                        (*leJoueur) = tabJoueur[1];
                    }
                }
            }   
        }
    
        (*leJoueur).joue=true;
    }else{
        if((*sens)==true)
        {
            (*sens)==false;
        }else
        {
            (*sens)==true;
        }
    }

    printf("Fin de votre tour\n");
    saisir_suivant();
}

//procédure pour la carte qui fait rejouer le joueur et change le sens du jeu
void rejouer_changement_sens(){

}

// fonction qui retourne un nombre aléatoire compris entre deux nombres
int nombre_aleatoire(int min, int max){
    int nbMystere;

    nbMystere = (rand() % (max-min+1)) + min;

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

// fonction pour saisir un nombre entre min et max
int saisir_suivant(){
 	int choix=1;
    do{
        printf("Appuyez sur 1 pour continuer\n");
        scanf("%d", &choix);
    }while(choix !=1);

    return choix;
}

void space(){
    printf("\n");
}