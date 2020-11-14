//Procédure qui va prendre un numéro dans la pioche, créer la carte et la mettre dans le deck des joueurs
void piocher_carte(_TJoueur * leJoueur, _TPioche pioche){
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

    _TCarte * newCarte;
    newCarte = (_TCarte*) malloc(sizeof(_TCarte));
    switch (laCarte)
    {
    case 1:
        (*newCarte).cout=1;
        (*newCarte).libelle[75]= "Supprimez 1 de vos des ";
        (*newCarte).effet=1;
        break;
    case 2:
        (*newCarte).cout=2;
        (*newCarte).libelle[75]= "Tous les joueurs donnent leurs des a leur voisin de gauche ou de droite. ";
        (*newCarte).effet=2;
        break;
    case 3:
        (*newCarte).cout=3;
        (*newCarte).libelle[75]= "Supprimez 2 de vos des ";
        (*newCarte).effet=3;
        break;
    case 4:
        (*newCarte).cout=3;
        (*newCarte).libelle[75]= "Donnez 1 de vos des au joueur de votre choix ";
        (*newCarte).effet=4;
        break;
    case 5:
        (*newCarte).cout=1;
        (*newCarte).libelle[75]= "Prenez 1 carte au joueur de votre choix ";
        (*newCarte).effet=5;
        break;
    case 6:
        (*newCarte).cout=1;
        (*newCarte).libelle[75]= "Le joueur de votre choix n’a plus qu’1 carte ";
        (*newCarte).effet=6;
        break;
    case 7:
        (*newCarte).cout=1;
        (*newCarte).libelle[75]= "Piochez 3 cartes dans la pioche ";
        (*newCarte).effet=7;
        break;
    case 8:
        (*newCarte).cout=2;
        (*newCarte).libelle[75]= "Tous les joueurs sauf vous n’ont plus que 2 cartes";
        (*newCarte).effet=8;
        break;
    case 9:
        (*newCarte).cout=0;
        (*newCarte).libelle[75]= "Le joueur de votre choix passe son tour";
        (*newCarte).effet=9;
        break;
    case 10:
        (*newCarte).cout=0;
        (*newCarte).libelle[75]= "Rejouez et changez de sens ";
        (*newCarte).effet=10;
        break;
    default:
        break;
    }

    (*newCarte).carteSuivante=NULL;

    _TCarte * aux;

    if((*leJoueur).cartes==NULL){
        (*leJoueur).cartes = newCarte;
    }else{
        aux= (*leJoueur).cartes;
        while (aux!=NULL)
        {
            if((*aux).carteSuivante==NULL){
                (*aux).carteSuivante = newCarte;
            }
        }
    }
}
