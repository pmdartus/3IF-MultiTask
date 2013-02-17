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

void Commande (char code);
// Mode d'emploi :
//  <code>  : Envoie de la commande normalisée recu par menu
//  - Permet le traitement des commandes 'F' et 'G' par l'interface
// Contrat :
//  -  La commande doit être normalisée en majuscule par le procedure menu 

void Commande (TypeVoie entree, TypeVoie sortie);
// Mode d'emploi :
//  <entree>  : Voie par laquelle la voiture entre dans le carefour
//  <sortie>  : Voie par laquelle la voiture sort du carefour
//  -  Permet de créer une voiture manuellement
// Contrat :
// Aucun

void Commande (TypeVoie voie, unsigned int duree);
// Mode d'emploi :
//  <voie>   : Selection de la voie dont on souhaite modifier le temps du feux
//  <duree>  : Duree d'attente du feux dans l'êtat vert
//  -  Modification de temps du feux dans l'état vert
// Contrat :
// Aucun

#endif // INTERFACE_H

