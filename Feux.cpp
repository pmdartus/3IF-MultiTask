/*************************************************************************
                           Feux  -  description
                             -------------------
    debut                : 06/02/2013
    copyright            : (C) nbuisson
    e-mail               : nicolas.buisson@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la tache <Feux> (fichier Feux.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include systeme
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include <sys/shm.h>
#include <sys/sem.h>

//------------------------------------------------------ Include personnel
#include "Feux.h"
#include "Mere.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int myIdEtatFeux;
static int myIdDuree;
static int myIdSem;

static Duree* myMemDuree;
static EtatFeux* myMemEtatFeux;

static struct sembuf reserver = {0, -1, 0};
static struct sembuf liberer = {0, 1, 0};

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

static void destruction(int noSignal)
// Mode d'emploi :
//
// Contrat : 
// 
// Algorithme :
//
{
	shmdt(myMemEtatFeux);
	shmdt(myMemDuree);
	exit(0);
} //----- fin de destruction

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
//type Nom ( liste de parametres )
// Algorithme :
//
//{
//} //----- fin de Nom

void ActiverFeux(int etatFeux, int duree, int sem)
// Mode d'emploi :
//
// Contrat :
//
{
	// Mise en place du handler de destruction
	struct sigaction fin;
	fin.sa_handler = destruction;
	sigemptyset(&fin.sa_mask);
	fin.sa_flags = 0;
	sigaction(SIGUSR2, &fin, NULL);

	myIdEtatFeux = etatFeux;
	myIdDuree = duree;
	myIdSem = sem;

	myMemEtatFeux = (EtatFeux*) shmat(myIdEtatFeux, NULL, 0);
	myMemDuree = (Duree*) shmat(myIdDuree, NULL, 0);

	for( ; ; );

} //----- fin de ActiverFeux

