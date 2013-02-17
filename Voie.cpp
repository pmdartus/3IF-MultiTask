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
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>

#include <Voiture.h>
#include <Outils.h>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Voie.h"
#include "Mere.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static unsigned int nVoie;
static EtatFeux * feux;

int myBAL;

static std::vector<pid_t> vectDeplacement;

//------------------------------------------------------ Fonctions privées

static void  FinTache ()
{

}

static void FinDeplacement ()
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

  // Traitement  de la fin de la tache
  struct sigaction finTache;
  finTache.sa_handler = FinTache();
  sigemptyset(&finTache.sa_mask);
  finTache.sa_flags = SA_RESTART;
  sigaction (SIGUSR2, &finTache, NULL);

  // Traitemet de la fin d'un déplacement
  struct sigaction finDeplacement;
  sigemptyset(&finDeplacement.sa_mask);
  finTache.sa_flags = FinDeplacement();
  sigaction (SIGCHLD, &finTache, NULL);


  // Attachement de la mémoire partagée
  feux = (EtatFeux *) shmat(idFeu, NULL, 0);

  //----------------------------
  // Moteur 
  //----------------------------

  for (;;)
  {

  }
} //----- fin de Voie

