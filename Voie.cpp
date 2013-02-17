/*************************************************************************
                           Voie  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) pmdartus
    e-mail               : pierre-marie.dartus@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la tache <Voie> (fichier Voie.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include systeme
#include <vector>
using namespace std;

//------------------------------------------------------ Include personnel
#include "Voie.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static unsigned int nVoie;
static EtatFeu * feux;

int myBAL;

static std::vector<pid_t> vectDeplacement;

//------------------------------------------------------ Fonctions privées
TypeVoie getVoie (int numVoie)
{
  case (numVoie)
  {
    1 : 
      return NORD;
      break;
    2 :
      return EST ;
      break;
    3 :
      return SUD ;
      break;
    4 :
      return OUEST;
      break;
  }
}

void  finTache ()
{

}

void finDeplacement ()
{

}

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Voie( unsigned int numVoie, int idFeu, int idFile )
// Algorithme :
//
{

  //----------------------------
  // Initialisation 
  //----------------------------

  nVoie = numVoie;
  struct sigaction finDeplacement;

  // Traitement  de la fin de la tache
  struct sigaction finTache;
  finTache.sa_handler = finTache;
  sigemptyset(&finTache.sa_mask);
  finTache.sa_flag = 0;
  sigaction (SIGUSR2, &finTache, NULL);

  // Traitemet de la fin d'un déplacement


  // Attachement de la mémoire partagée
  feux = (EtatFeu *) shmat(idFeu, NULL, 0);

  //----------------------------
  // Moteur 
  //----------------------------

  for (;;)
  {

  }
} //----- fin de Voie

