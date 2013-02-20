/*************************************************************************
                           Voie  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) pmdartus
    e-mail               : pierre-marie.dartus@insa-lyon.fr
*************************************************************************/

//---------- Interface de la tache <Voie> (fichier Voie.h) -------
#if ! defined ( VOIE_H )
#define VOIE_H

//------------------------------------------------------------------------
// Rôle de la tache <Voie>
// Affichage et déplacement des voitures pour chaque voies
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisees

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Voie( TypeVoie numVoie, int idFeu, int idFile );
// Mode d'emploi :
//	<numVoie>	: position de la voie dans le carrefour
//	<idFeu>		: id de la zone mémoire partagée contenant l'état des 
//				  feux
//	<idFile>	: id de la boite au lettre contenant les MsgVoitures
// - Réception des messages et traitement des voitures
// Contrat :
// Aucun

#endif // VOIE_H

