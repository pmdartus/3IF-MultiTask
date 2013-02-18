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

#include <Outils.h>

using namespace std;

//------------------------------------------------------ Include personnel
#include "Feux.h"
#include "Mere.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

static const int DUREE_ORANGE = 3;
static const int DUREE_ROUGE = 2;
static const int DUREE_ARRET = DUREE_ORANGE + DUREE_ROUGE;
static const char VERT[] = "Vert  \0";
static const char ORANGE[] = "Orange\0";
static const char ROUGE[] = "Rouge \0";

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int myIdEtatFeux;
static int myIdDuree;
static int myIdSem;

static int dureeEO;
static int dureeNS;

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

static void afficherDuree()
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	Afficher(DUREE_AXE_NS, dureeNS, STANDARD, NORMALE);
	Afficher(DUREE_AXE_EO, dureeEO, STANDARD, NORMALE);
} //----- fin de afficherDuree

static void afficherNS(int duree, int dureeSup)
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	Afficher(TEMPS_AXE_NS, duree, STANDARD, NORMALE);
	Afficher(TEMPS_AXE_EO, duree + dureeSup, STANDARD, NORMALE);
} //----- fin de afficherNS

static void afficherEO(int duree, int dureeSup)
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	Afficher(TEMPS_AXE_EO, duree, STANDARD, NORMALE);
	Afficher(TEMPS_AXE_NS, duree + dureeSup, STANDARD, NORMALE);
} //----- fin de afficherEO

static void majDuree()
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	semop(myIdDuree, &reserver, 1);
	dureeEO = myMemDuree->eO;
	dureeNS = myMemDuree->nS;
	semop(myIdDuree, &liberer, 1);
} //----- fin de majDuree

static void initialisation()
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	// Attachement des zones de mémoires partagées
	myMemEtatFeux = (EtatFeux*) shmat(myIdEtatFeux, NULL, 0);
	myMemDuree = (Duree*) shmat(myIdDuree, NULL, 0);

	// Initialisation des durées des feux
	majDuree();

	// Les feux de l'axe Nord-Sud se mettent au vert
	myMemDuree->nS = true;

	// Affichage des couleurs initiales
	Afficher(COULEUR_AXE_NS, VERT, GRAS, INVERSE);
	Afficher(COULEUR_AXE_EO, ROUGE, GRAS, INVERSE);

	// Affichage des durées initiales
	Afficher(TEMPS_AXE_NS, dureeNS, STANDARD, NORMALE);
	Afficher(TEMPS_AXE_EO, dureeNS + DUREE_ARRET, STANDARD, NORMALE);
	afficherDuree();
} //----- fin de initialisation

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

	// Initialisation des IDs
	myIdEtatFeux = etatFeux;
	myIdDuree = duree;
	myIdSem = sem;


	initialisation();

	for( ; ; )
	{
		majDuree();
		afficherDuree();

		Afficher(COULEUR_AXE_NS, VERT, STANDARD, NORMALE);
		for(int i = dureeNS ; i > 0 ; i--)
		{
			afficherNS(i, DUREE_ARRET);
			sleep(1);
		}

		Afficher(COULEUR_AXE_NS, ORANGE, STANDARD, NORMALE);
		// Inversion de l'état de l'état du feu N-S
		myMemEtatFeux->nS ^= true;
		for(int i = 3 ; i > 0 ; i--)
		{
			afficherNS(i, DUREE_ROUGE);
			sleep(1);
		}

		Afficher(COULEUR_AXE_NS, ROUGE, GRAS, INVERSE);
		for(int i = 2 ; i > 0 ; i--)
		{
			afficherNS(i, 0);
			sleep(1);
		}

		majDuree();
		afficherDuree();

		// Inversion de l'état du feux E-O
		myMemEtatFeux->eO ^= true;

		Afficher(COULEUR_AXE_EO, VERT, STANDARD, NORMALE);
		for(int i = dureeEO ; i > 0 ; i--)
		{
			afficherEO(i, DUREE_ARRET);
			sleep(1);
		}

		Afficher(COULEUR_AXE_EO, ORANGE, STANDARD, NORMALE);
		// Inversion de l'état de l'état du feux E-O
		myMemEtatFeux->eO ^= true;
		for(int i = 3 ; i > 0 ; i--)
		{
			afficherEO(i, DUREE_ROUGE);
			sleep(1);
		}

		Afficher(COULEUR_AXE_EO, ROUGE, GRAS, INVERSE);
		for(int i = 2 ; i > 0 ; i--)
		{
			afficherEO(i, 0);
			sleep(1);
		}

		// Inversion de l'état de l'état du feu N-S
		myMemEtatFeux->nS ^= true;
	}

} //----- fin de ActiverFeux

