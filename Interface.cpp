/*************************************************************************
                           Interface  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) pmdartus
    e-mail               : pierre-marie.dartus@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la tache <Interface> (fichier Interface.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include systeme
#include <generateur.h>
#include <voiture.h>
#include <menu.h>
#include <outils.h>

#include <stdlib.h>
#include <signal.h>
#include <sys/type.h>
#include <sys/shm>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Interface.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static bool bGeneLaunched = false;

//------------------------------------------------------ Fonctions privées
//static type nom ( liste de parametres )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
//{
//} //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void InitInterface ( pid_t gene, int idSem, int memDuree, int idFile )
// Algorithme : Voir le dossier de spec
{

  // Attachement de la mémoire partagé

  for (;;)
  {
  }
}

void Commande (char code)
{
  if (code == 'F')
    // Déclanchement de la fin de l'application sur le réception du code 'F'
  {
    kill ( getppid(), SIGUSR2);
    exit (0);
  }
  else if ( code == 'G')
    // Arret du generateur si actif actuellement, sinon mise en route du gene 
  {
    if (bGeneLaunched==true)
    {
      kill (gene, SIGSTOP);
    }
    else 
    {
      kill (gene, SIGCONT);
    }
  }
  else
    // code non reconnus -> Fermeture de l'application 
  {
    exit (1);
  }
}

void Commande (TypeVoie entree, TypeVoie sortie)
{

}

void Commande (TypeVoie voie, unsigned int duree)
{

}