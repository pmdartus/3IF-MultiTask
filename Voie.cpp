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
#include <sys/msg.h>
#include <sys/wait.h>

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
static TypeVoie nVoie;
static EtatFeux * feux;

int myBAL;

static std::vector<pid_t> vectDeplacement;

//------------------------------------------------------ Fonctions privées

static void  finTache (int typeSignal)
{
  if (typeSignal == SIGUSR2)
  {
    // Suppression des déplacements restants
    std::vector<pid_t> ::iterator it=vectDeplacement.begin();
    while(it!=vectDeplacement.end())
    {
      kill((*it),SIGUSR2);
      waitpid((*it),NULL,0);
      it++;
    }

    // Supression handlers


    // Détachement de la mémoire
    shmdt (feux);

    // Fin de la tache
    exit(0);
  }
}

static void finDeplacement (int typeSignal)
{
  if (typeSignal == SIGCHLD)
  {
    // Supression dans la liste des deplacement de la voiture ayant finis
    // son déplacement
    int etat;
    pid_t pidFils=wait(&etat);
    std::vector<pid_t> ::iterator it;
    while ((*it)!=pidFils)
    {
      it++;
    }
    vectDeplacement.erase(it);
  }
}

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Voie( TypeVoie numVoie, int idFeu, int idFile )
// Algorithme :
//
{

  //----------------------------
  // Initialisation 
  //----------------------------

  nVoie = numVoie;

  // Traitement  de la fin de la tache
  
  struct sigaction actionFinTache;
  actionFinTache.sa_handler = finTache;
  actionFinTache.sa_flags = 0;
  sigaction (SIGUSR2, &actionFinTache, NULL);

  // Traitemet de la fin d'un déplacement

  struct sigaction actionFinDeplacement;
  actionFinTache.sa_handler = finDeplacement;
  actionFinTache.sa_flags = 0;
  sigaction (SIGCHLD, &actionFinTache, NULL);



  // Attachement de la mémoire partagée
  feux = (EtatFeux *) shmat(idFeu, NULL, 0);

  // Réception d'un message
  struct MsgVoiture msg;

  //----------------------------
  // Moteur 
  //----------------------------

  for (;;)
  {
    // Attente de la prochaine voiture à traiter par la voie en question
    // Etat d'attente bloquant
    while ( msgrcv(myBAL, &msg, TAILLE_MSG_VOITURE, (long)(nVoie), 0) < 0  )
    {
      
      DessinerVoitureFeu(msg.uneVoiture.numero, msg.uneVoiture.entree, msg.uneVoiture.sortie);
      OperationVoie (MOINS, numVoie);

      if (numVoie == NORD || numVoie == SUD)
      {
        if (feux->nS)
        {
          pid_t voitureBouge =DeplacerVoiture(msg.uneVoiture.numero, msg.uneVoiture.entree, msg.uneVoiture.sortie);
          vectDeplacement.push_back(voitureBouge);
        }
      }
      else
      {
        if (feux->eO)
        {
          pid_t voitureBouge =DeplacerVoiture(msg.uneVoiture.numero, msg.uneVoiture.entree, msg.uneVoiture.sortie);
          vectDeplacement.push_back(voitureBouge);
        }
      }

    }

  }
} //----- fin de Voie

