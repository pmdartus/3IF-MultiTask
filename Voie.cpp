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
#include <unistd.h>

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

static void  handlerFinTache (int typeSignal)
// Algorithme :
// Parcourt de la structure de donnée, et on tue toutes les taches de 
// déplacement en cours, avant de quitter la tache
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

    // Détachement de la mémoire
    shmdt (feux);

    // Fin de la tache
    exit(0);
  }
}

static void handlerFinDeplacement (int typeSignal)
// Algorithme :
// Recherche du pid de la tache ayant envoyé le signal, et suppression
// du pid de la structure de donnée existante
{
  if (typeSignal == SIGCHLD)
  {
    // Supression dans la liste des deplacement de la voiture ayant finis
    // son déplacement
    int etat;
    pid_t pidFils=wait(&etat);
    std::vector<pid_t> ::iterator it;
    it=vectDeplacement.begin();
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
// Phase d'initilisation triviale
// Phase moteur : attente d'une voiture dans la BAL pui affichage de celle - ci
// Une fois affichée, on attend que le feux passe au vert et on déplace la voiture
// après l'avoir ajoutée à la structure de donnée stoquant les voitures en déplacement
{

  //----------------------------
  // Initialisation 
  //----------------------------

  nVoie = numVoie;
  myBAL = idFile ;

  // Traitement  de la fin de la tache
  
  struct sigaction actionFinTache;
  actionFinTache.sa_handler = handlerFinTache;
  actionFinTache.sa_flags = 0;
  sigaction (SIGUSR2, &actionFinTache, NULL);

  // Traitemet de la fin d'un déplacement

  struct sigaction actionFinDeplacement;
  actionFinTache.sa_handler = handlerFinDeplacement;
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
    if(msgrcv(myBAL, &msg, TAILLE_MSG_VOITURE, nVoie, 1)!=-1)
    {
      // Display     
      DessinerVoitureFeu(msg.uneVoiture.numero, msg.uneVoiture.entree, msg.uneVoiture.sortie);
      OperationVoie (MOINS, nVoie);

      if (nVoie == NORD || nVoie == SUD)
      {
        // Attente du passage du feux au vert
        while (!feux->nS)
        {
          sleep(1);
        }
        
        // Création de la tache de déplacement de la voiture et ajout à la liste des tache en execution
        pid_t voitureBouge =DeplacerVoiture(msg.uneVoiture.numero, msg.uneVoiture.entree, msg.uneVoiture.sortie);
        vectDeplacement.push_back(voitureBouge);
      }
      else
      {
        // Attente du passage du feux au vert
        while (!feux->eO)
        {
          sleep(1);
        }

        // Création de la tache de déplacement de la voiture et ajout à la liste des tache en execution
        pid_t voitureBouge =DeplacerVoiture(msg.uneVoiture.numero, msg.uneVoiture.entree, msg.uneVoiture.sortie);
        vectDeplacement.push_back(voitureBouge);
      }
    }

  }
} //----- fin de Voie

