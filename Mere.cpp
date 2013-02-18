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
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/sem.h>

#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include <Outils.h>
#include <Heure.h>
#include <Generateur.h>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Mere.h"
#include "Interface.h"
#include "Voie.h"
#include "Feux.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées

static void creerMemoires(int& etatFeux, int& duree)
// Mode d'emploi :
// Créé et initialise les zones de mémoires partagées
// <etatFeux> : id de la zone mémoire partagée contenant l'état des feux
// <duree> : id de la zone mémoire partagée contenant la durée des feux
//
// Alogrithme :
// Trivial
{
    // Création de la zone de mémoire EtatFeux
	etatFeux = shmget(IPC_PRIVATE, sizeof(EtatFeux), 0666 | IPC_CREAT);

	// Création de la zone de mémoire Duree
	duree = shmget(IPC_PRIVATE, sizeof(Duree), 0666 | IPC_CREAT);

	// Initialisation de EtatFeux
	EtatFeux* memEtatFeux = (EtatFeux*) shmat(etatFeux, NULL, 0);
	memEtatFeux->eO = false;
	memEtatFeux->nS = false;
	shmdt(memEtatFeux);

	// Initialisation de Duree
	Duree* memDuree = (Duree*) shmat(duree, NULL, 0);
	memDuree->eO = 12;
	memDuree->nS = 18;
	shmdt(memDuree);

} //----- fin de creerMemoires

static void creerBAL(int& fileVoitures)
// Mode d'emploi :
// Crée les boites aux lettres contenant les files de voitures
// <fileVoitures> : id de la BAL contenant les voitures
//
// Algorithme :
// Trivial
{
    fileVoitures = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
} //----- fin de creerBAL

static void detruireMemoires(int duree, int etatFeux)
// Mode d'emploi :
// Détruit les zones de mémoires partagées
// <etatFeux> : id de la zone mémoire partagée contenant l'état des feux
// <duree> : id de la zone mémoire partagée contenant la durée des feux
//
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
// <fileVoitures> : id de la BAL contenant les voitures
//
// Algorithme :
// Trivial
{
    msgctl(fileVoitures, IPC_RMID, 0);
} //----- fin de detruireBAL

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

int main()
// Mode d'emploi :
// Corps du programme
//
// Algorithme :
// - Mise en place des zones de mémoires partagées, boîtes aux lettres et sémaphores
// - Création du générateur et de l'heure
// - Création du feux
// - Création des voies
// - Création de l'interface
// - Attente de la fin de l'interface
// - Destruction dans l'ordre inverse de création
{
    pid_t pidHeure;
    pid_t pidInterface;
    pid_t pidVoie[4];
    pid_t pidFeux;
    pid_t pidGenerateur;

	int idSem;
    int idEtatFeux;
    int idDuree;
    int idFileVoiture;

	// Masquage de SIGUSR2 et SIGCHLD
	struct sigaction maskUSR2;
	struct sigaction maskCHLD;

	maskUSR2.sa_handler = SIG_IGN;
	sigemptyset(&maskUSR2.sa_mask);
	maskUSR2.sa_flags = 0;
	sigaction(SIGUSR2,&maskUSR2, NULL);

	maskCHLD.sa_handler = SIG_IGN;
	sigemptyset(&maskCHLD.sa_mask);
	maskCHLD.sa_flags = 0;
	sigaction(SIGCHLD,&maskCHLD, NULL);
	// Fin de l'opération de masquage

    // Création des zones mémoires
    creerMemoires(idEtatFeux, idDuree);

    // Création des boites aux lettres
    creerBAL(idFileVoiture);

	// Création du sémaphore d'exclusion mutuelle
	idSem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	semctl(idSem, 0, SETVAL, 1);


    // Initialisation de l'application
    InitialiserApplication(XTERM);

    // Création de l'heure
    pidHeure = CreerEtActiverHeure();

	// Création du générateur
	pidGenerateur = CreerEtActiverGenerateur(0, idFileVoiture);

	// Création de Feux
	if((pidFeux = fork()) == 0)
	{
		ActiverFeux(idEtatFeux, idDuree, idSem);
	}
	else
	{
		// Création des voies
		if((pidVoie[0] = fork()) == 0)
		{
			Voie(NORD, idEtatFeux, idFileVoiture);
		}
		else
		{
			if((pidVoie[1] = fork()) == 0)
			{
				Voie(SUD, idEtatFeux, idFileVoiture);
			}
			else
			{
				if((pidVoie[2] = fork()) == 0)
				{
					Voie(EST, idEtatFeux, idFileVoiture);
				}
				else
				{
					if((pidVoie[3] = fork()) == 0)
					{
						Voie(OUEST, idEtatFeux, idFileVoiture);
					}
					else
					{

						// Lancement de l'interface
						if((pidInterface = fork()) == 0)
						{
							Interface(pidGenerateur, idSem, idDuree, idFileVoiture); 
						}
						else
						{

							// Synchronisation de fin
							waitpid(pidInterface, NULL, 0);

							// Destruction des voies
							for(int i = 0 ; i < 4 ; i++)
							{
								kill(pidVoie[i], SIGUSR2);
								waitpid(pidVoie[i], NULL, 0);
							}

							// Destruction de Feux
							kill(pidFeux, SIGUSR2);
							waitpid(pidFeux, NULL, 0);

							// Destruction du générateur
							kill(pidGenerateur, SIGCONT);
							kill(pidGenerateur, SIGUSR2);
							waitpid(pidGenerateur, NULL, 0);

							// Destruction de l'heure
							kill(pidHeure, SIGUSR2);
							waitpid(pidHeure, NULL, 0);

							// Destruction du sémaphore
							semctl(idSem, 0, IPC_RMID, 0);

							// Destruction des zones de mémoires partagées
							detruireMemoires(idEtatFeux, idDuree);

							// Destruction des boites aux lettres
							detruireBAL(idFileVoiture);

							// Destruction de l'interface
							TerminerApplication(true);
        
							exit(0);
						}
					}
				}
			}
		}
	}

	return 0;
} //----- fin de main

