// fonction qui retourne le nombre de cartes d’un joueur
int nombre_carte(_TJoueur leJoueur){
    int nbCarte; //Nombre de carte 
    _TCarte * aux;
    (*leJoueur).carte = aux;
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
            aux = aux.carteSuivante;
        }
    }
    return nbCarte;
}