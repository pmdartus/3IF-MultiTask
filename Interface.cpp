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
#include <sys/msb.h>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Interface.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes
NUM_VOITURE_MAX = 299;
//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static bool bGeneLaunched;
static unsigned int numVoiture;

static Duree * dureeFeux;
static int mySem;

static int myBAL;

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

void Interface ( pid_t gene, int idSem, int memDuree, int idFile )
// Algorithme : Voir le dossier de spec
{
  //----------------------------
  // Initialisation 
  //----------------------------

  // Init variables
  bGeneLaunched = false;
  numVoiture = 1;
  myBal = idFile;

  // Attachement de la mémoire partagé
  dureeFeux = (Duree *) shmat(memDuree, NULL, 0);

  // Gestion des semaphores


  //----------------------------
  // Moteur 
  //----------------------------

  for (;;)
  {
    Menu();
  }
}

void Commande (char code)
// Algorithme : Trivial, une commande implique une action
{
  switch ( code )
  {
    case 'F':
      // Destruction de la tache en cour et fin de l'application
      exit (0);
    break;

    case 'G':
    // Arret du generateur si actif actuellement, sinon mise en route du gene 
      if (bGeneLaunched==true)
      {
        kill (gene, SIGSTOP);
      }
      else 
      {
        kill (gene, SIGCONT);
      }
    break;

    default:
    // Commande non reconnue par le parsseur
      Effacer(MESSAGE);
      Afficher(MESSAGE, "ERREUR | Commande non conforme");
    break;
}

void Commande (TypeVoie entree, TypeVoie sortie)
{
  Voiture aVoiture;
  MsgVoiture msg;
  size_t msgSize;

  // Modification du numero de la voiture a creer
  numVoiture<NUM_VOITURE_MAX : numVoiture += 1 , numVoiture = 1 ;

  // Mise en place de la voiture
  aVoiture.entree = entree;
  aVoiture.sortie = sortie;
  aVoiture.numero  = numVoiture;

  // Mise en place du message
  msg.uneVoiture = aVoiture;
  msg.type = msg.uneVoiture.entree;

  msgSize = size_of(MsgVoiture);

  msgsnd(myBAL, &msg, msgSize, 0);
}

void Commande (TypeVoie voie, unsigned int duree)
{

}