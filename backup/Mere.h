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
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisees

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

struct EtatFeux
{
    bool eO;
    bool nS;
};

struct Duree
{
    int eO;
    int nS;
};

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
// type Nom ( liste de parametres );
// Mode d'emploi :
//
// Contrat :
//

int main();
// Mode d'emploi :
// Code principal de l'application
// Contrat :
// Aucun

#endif // MERE_H

