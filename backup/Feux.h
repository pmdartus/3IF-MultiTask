/*************************************************************************
                           Feux  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) nbuisson
    e-mail               : nicolas.buisson@insa-lyon.fr
*************************************************************************/

//---------- Interface de la tache <Feux> (fichier Feux.h) -------
#if ! defined ( FEUX_H )
#define FEUX_H

//------------------------------------------------------------------------
// Rôle de la tache <Feux>
// Gère le changement de couleur et d'état des feux.
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisees

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void ActiverFeux(int etatFeux, int duree, int sem);
// Mode d'emploi :
//	<etatFeux>	: id de la zone mémoire partagée contenant l'état des feux
//	<duree>		: id de la zone mémoire partagée contenant la durée des 
//				  feux
//	<sem>		: id du sémaphore d'exclusion mutuelle liée à la zone de 
//				  mémoire durée
// - lance les feux
//
// Contrat :
// - les zones mémoires doivent être préalablement créée

#endif // FEUX_H

