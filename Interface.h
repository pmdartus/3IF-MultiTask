/*************************************************************************
                           Interface  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) pmdartus
    e-mail               : pierre-marie.dartus@insa-lyon.fr
*************************************************************************/

//---------- Interface de la tache <Interface> (fichier Interface.h) -------
#if ! defined ( INTERFACE_H )
#define INTERFACE_H

//------------------------------------------------------------------------
// Rôle de la tache <Interface>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisees

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Interface ( pid_t gene, int idSem, int memDuree, int idFile );
// Mode d'emploi :
//  <gene>    : le pid du générateur de voiture
//  <idSem>   : L'identificateur de sémaphore protégent la mémoire partagé
//              de la durée des feux
// <memDuree> : L'identificateur de la memoire partagée contenant la duree
//              des feux
// <idFile>   : L'identificateur de la BAL contenant les voitures en attente
//
//  - Lancement de la tache interface par la mêre
// Contrat :
//  - Toutes les ressources et structures de données doivent être crées en amont 
//    par le mêre avant le lancement de la tache interface

#endif // INTERFACE_H

