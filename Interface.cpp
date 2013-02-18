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
#include <Generateur.h>
#include <Voiture.h>
#include <Menu.h>
#include <Outils.h>

#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Interface.h"
#include "Mere.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static bool bGeneLaunched;
static unsigned int numVoiture;

static Duree * dureeFeux;
static int mySem;

static int myBAL;

static pid_t pidGene;

//------------------------------------------------------ Fonctions privées
static void FermerInterface ()
// Algorithme : Trivial
{

  Effacer(MESSAGE);
  Afficher(MESSAGE, "Fermeture");
  // Detache la mémorie partagée avec le prcessus
  shmdt(dureeFeux);

  // On tue la tache dans l'oeuf
  exit (0);
}

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void Interface ( pid_t gene, int idSem, int memDuree, int idFile )
// Algorithme : Trivial
{
  //----------------------------
  // Initialisation 
  //----------------------------

  // Init variables
  bGeneLaunched = false;
  numVoiture = 1;
  myBAL = idFile;
  pidGene = gene;

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
} //------ Fin d'Interface

void Commande (char code)
// Algorithme : Trivial, une commande implique une action
{
  switch ( code )
  {
    case 'F':
      // Destruction de la tache en cour et fin de l'application
      FermerInterface();
    break;

    case 'G':
    // Arret du generateur si actif actuellement, sinon mise en route du gene 
      if (bGeneLaunched==true)
      {
        kill (pidGene, SIGSTOP);
        Effacer(ETAT_GENERATEUR);
        Afficher(ETAT_GENERATEUR, "OFF");
        Effacer(MESSAGE);
        Afficher(MESSAGE, "Arret du generateur");
        bGeneLaunched = false ;
      }
      else 
      {
        kill (pidGene, SIGCONT);
        Effacer(ETAT_GENERATEUR);
        Afficher(ETAT_GENERATEUR, " ON");
        Effacer(MESSAGE);
        Afficher(MESSAGE, "Mise en route du generateur");
        bGeneLaunched = true ;
      }
    break;

    default:
    // Commande non reconnue par le parsseur
      Effacer(MESSAGE);
      Afficher(MESSAGE, "ERREUR | Commande non conforme");
    break;
  }
} //------ Fin de Commande (char code)

void Commande (TypeVoie entree, TypeVoie sortie)
// Algorithme : Creation de la voiture, du message puis ajout dans la BAL
{
  Voiture aVoiture;
  MsgVoiture msg;
  size_t msgSize;

  // Modification du numero de la voiture a creer
  if (numVoiture < NUM_VOITURE_MAX)
  {
    numVoiture += 1 ;
  }
  else 
  {
    numVoiture = 1 ;
  }

  // Mise en place de la voiture
  aVoiture.entree = entree;
  aVoiture.sortie = sortie;
  aVoiture.numero  = numVoiture;

  // Mise en place du message
  msg.uneVoiture = aVoiture;
  msg.type = msg.uneVoiture.entree;

  msgSize = sizeof(msg);

  msgsnd(myBAL, &msg, msgSize, 0);

  // Display
  Effacer(MESSAGE);
  Afficher(MESSAGE, "Ajout de la voiture à la file");
} //------ Fin de Commande (TypeVoie entree, TypeVoie sortie)

void Commande (TypeVoie voie, unsigned int duree)
// Algorithme : Prise du jeton, modification de la duree puis le jeton
// est reposé
{
  //Creation des SemBuf
  struct sembuf duree_V;
  duree_V.sem_num = mySem;
  duree_V.sem_op = 1;
  duree_V.sem_flg = 0;

  struct sembuf duree_P;
  duree_P.sem_num = mySem;
  duree_P.sem_op = -1;
  duree_P.sem_flg = 0;
  

  if (voie == NORD || voie == SUD)
  {
    semop(mySem, &duree_P, 1);
    dureeFeux->nS = duree;
    semop(mySem, &duree_V, 1);

    // Display
    Effacer(MESSAGE);
    Afficher(MESSAGE, "Axe NS modification de la duree du Feux");
  }
  else if (voie == OUEST || voie == EST)
  {
    semop(mySem, &duree_P, 1);
    dureeFeux->eO = duree;
    semop(mySem, &duree_V, 1);

    // Display
    Effacer(MESSAGE);
    Afficher(MESSAGE, "Axe EO modification de la duree du Feux");
  }
  else
  {
    // Nous avons un grâve problème !!
    Effacer(MESSAGE);
    Afficher(MESSAGE, "ERREUR | La voie demandée n'est pas valide");
    exit (1);
  }
} //------ Fin de Commande ( TypeVoie voie, unsigned int duree)