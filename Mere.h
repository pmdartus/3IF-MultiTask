/*************************************************************************
                           Mere  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) nbuisson
    e-mail               : nicolas.buisson@insa-lyon.fr
*************************************************************************/

//---------- Interface de la tache <Mere> (fichier Mere.h) -------
#if ! defined ( MERE_H )
#define MERE_H

//------------------------------------------------------------------------
// Rôle de la tache <Mere>
// Tâche moteur du programme. Gère la création et la destruction des
// tâches filles.
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisees

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

// Etats des Feux
// état :
//	- faux : feu rouge/orange
//	- vrai : feu vert
//
// eO : axe Est-Ouest
// nS : axe Nord-Sud
struct EtatFeux
{
    bool eO;
    bool nS;
};


// Durée des Feux
// eO : axe Est-Ouest
// nS : axe Nord-Sud
struct Duree
{
    int eO;
    int nS;
};


//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

int main();
// Mode d'emploi :
// Code principal de l'application
// Contrat :
// Aucun

#endif // MERE_H

