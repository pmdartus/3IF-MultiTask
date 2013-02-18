/*************************************************************************
                           Mere  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) nbuisson
    e-mail               : nicolas.buisson@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la tache <Mere> (fichier Mere.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include systeme
#include <unistd.h>

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/sem.h>

#include <signal.h>
#include <stdlib.h>

#include <Outils.h>
#include <Heure.h>
#include <Generateur.h>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Mere.h"
#include "Interface.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées

static void creerMemoires(int& etatFeux, int& duree)
// Mode d'emploi :
// Crée les zones de mémoires partagées
// Alogrithme :
// Trivial
{
    // Création de la zone de mémoire EtatFeux
	etatFeux = shmget(IPC_PRIVATE, sizeof(EtatFeux), 0666 | IPC_CREAT);

	// Création de la zone de mémoire Duree
	duree = shmget(IPC_PRIVATE, sizeof(Duree), 0666 | IPC_CREAT);
} //----- fin de creerMemoires

static void creerBAL(int& fileVoitures)
// Mode d'emploi :
// Crée les boites aux lettres contenant les files de voitures
// Algorithme :
// Trivial
{
    fileVoitures = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
} //----- fin de creerBAL

static void detruireMemoires(int duree, int etatFeux)
// Mode d'emploi :
// Détruit les zones de mémoires partagées
// Algorithme :
// Trivial
{
    // Destruction de EtatFeux
    shmctl(etatFeux, IPC_RMID, 0);

    // Destruction de Duree
    shmctl(duree, IPC_RMID, 0);
} //----- fin de detruireMemoires

static void detruireBAL(int fileVoitures)
// Mode d'emploi :
// Détruit les boites aux lettres
// Algorithme :
// Trivial
{
    msgctl(fileVoitures, IPC_RMID, 0);
} //----- fin de detruireBAL

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

int main()
{
    pid_t pidHeure;
    pid_t pidInterface;
    pid_t pidVoie[4];
    pid_t pidFeux;
    pid_t pidDeplacement;
    pid_t pidGenerateur;

	int idSemFile;
    int idEtatFeux;
    int idDuree;
    int idFileVoiture;

	// Masquage de SIGUSR2 et SIGCHLD
	struct sigaction action;

	action.sa_handler = SIG_IGN;

	sigaction(SIGUSR2, &action, NULL);
	sigaction(SIGCHLD, &action, NULL);

    // Création des zones mémoires
    creerMemoires(idEtatFeux, idDuree);

    // Création des boites aux lettres
    creerBAL(idFileVoiture);

	// Création du sémaphore d'exclusion mutuelle
	//idSemFile = semget(IPC_PRIVATE, 1, IPC_CREAT);


    // Initialisation de l'application
    InitialiserApplication(XTERM);

    // Création de l'heure
    pidHeure = CreerEtActiverHeure();

	// Création du générateur
	pidGenerateur = CreerEtActiverGenerateur(0, idFileVoiture);

    if((pidInterface = fork()) == 0)
	{
		Interface(pidGenerateur, 1, idDuree, idFileVoiture); 
	}
	else
	{

		waitpid(pidInterface, NULL, 0);

		// Destruction du générateur
		kill(pidGenerateur, SIGCONT);
		kill(pidGenerateur, SIGUSR2);
		waitpid(pidGenerateur, NULL, 0);

		// Destruction de l'heure
		kill(pidHeure, SIGUSR2);
		waitpid(pidHeure, NULL, 0);

		// Destruction des zones de mémoires partagées
		detruireMemoires(idEtatFeux, idDuree);

		// Destruction des boites aux lettres
		detruireBAL(idFileVoiture);

        // Destruction de l'interface
        TerminerApplication(true);
        
		exit(0);
	}

	return 0;
} //----- fin de main

