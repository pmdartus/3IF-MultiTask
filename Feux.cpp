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

static void destruction(int noSignal)
// Mode d'emploi :
// Provoque la fin propre de la tâche
// Contrat : 
// Aucun
// Algorithme :
// Détache les zones de mémoires Duree et EtatFeux puis
// termine la tâche
{
	shmdt(myMemEtatFeux);
	shmdt(myMemDuree);
	exit(0);
} //----- fin de destruction

static void afficherDuree()
// Mode d'emploi :
// Affiche les durées des feux dans les zones adéquates
// Contrat :
// Aucun
// Algorithme :
// Trivial
{
	Afficher(DUREE_AXE_NS, dureeNS, STANDARD, NORMALE);
	Afficher(DUREE_AXE_EO, dureeEO, STANDARD, NORMALE);
} //----- fin de afficherDuree

static void afficherNS(int duree, int dureeSup)
// Mode d'emploi :
// Affiche le temps duree sur la zone d'affichage de l'axe N-S
// Affiche le temps duree+dureeSup sur la zone d'affiche de l'axe E-O
// Contrat :
// Aucun
// Algorithme :
// Trivial
{
	Afficher(TEMPS_AXE_NS, duree, STANDARD, NORMALE);
	Afficher(TEMPS_AXE_EO, duree + dureeSup, STANDARD, NORMALE);
} //----- fin de afficherNS

static void afficherEO(int duree, int dureeSup)
// Mode d'emploi :
// Affiche le temps duree sur la zone d'affichage de l'axe E-O
// Affiche le temps duree+dureeSup sur la zone d'affiche de l'axe N-S
// Contrat :
// Aucun
// Algorithme :
// Trivial
{
	Afficher(TEMPS_AXE_EO, duree, STANDARD, NORMALE);
	Afficher(TEMPS_AXE_NS, duree + dureeSup, STANDARD, NORMALE);
} //----- fin de afficherEO

static void majDuree()
// Mode d'emploi :
// Mets à jour les durées des feux E-O et N-S
// Contrat :
// Aucun
// Algorithme :
//	- Prends un jeton du sémaphore de protection de la zone de mémoire Duree
//	- Lis les valeurs eO et nS de Duree et les stocke dans dureeEO et dureeNS
//	- Rends le jeton du sémaphore
{
	semop(myIdDuree, &reserver, 1);
	dureeEO = myMemDuree->eO;
	dureeNS = myMemDuree->nS;
	semop(myIdDuree, &liberer, 1);
} //----- fin de majDuree

static void initialisation()
// Mode d'emploi :
// Initialise la tâche Feux
// Contrat :
// Les variables myIdEtatFeux, myIdDuree et myIdSem doivent être déjà initialisées
// Algorithme :
//	- Attache les zones mémoires EtatFeux et Duree
//	- Mets à jour les durées des feux
//	- Passe l'état du feu N-S à vrai
//	- Affiche les couleurs des feux
//	- Affiche les temps initiaux des feux
//	- Affiche les durées initiales des feux
{
	// Attachement des zones de mémoires partagées
	myMemEtatFeux = (EtatFeux*) shmat(myIdEtatFeux, NULL, 0);
	myMemDuree = (Duree*) shmat(myIdDuree, NULL, 0);

	// Initialisation des durées des feux
	majDuree();

	// Les feux de l'axe Nord-Sud se mettent au vert
	myMemEtatFeux->nS = true;

	// Affichage des couleurs initiales
	Afficher(COULEUR_AXE_NS, VERT, GRAS, INVERSE);
	Afficher(COULEUR_AXE_EO, ROUGE, GRAS, INVERSE);

	// Affichage des temps initiaux
	Afficher(TEMPS_AXE_NS, dureeNS, STANDARD, NORMALE);
	Afficher(TEMPS_AXE_EO, dureeNS + DUREE_ARRET, STANDARD, NORMALE);

	// Affichage des durées initiales
	afficherDuree();
} //----- fin de initialisation

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void ActiverFeux(int etatFeux, int duree, int sem)
// Mode d'emploi :
// Lance le fonctionnement de la tâche Feux
// Contrat :
// Les zones de mémoires EtatFeux, Duree et le mutex de Duree doivent
// être déjà créés.
{
	//------------------//
	//  Initialisation  //
	//------------------//

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


	//------------------//
	//      Moteur      //
	//------------------//

	for( ; ; )
	{
		// Mise à jour des durées
		majDuree();
		afficherDuree();

		Afficher(COULEUR_AXE_NS, VERT, STANDARD, NORMALE);
		myMemEtatFeux->nS = true;
		for(int i = dureeNS ; i > 0 ; i--)
		{
			afficherNS(i, DUREE_ARRET);
			sleep(1);
		}

		Afficher(COULEUR_AXE_NS, ORANGE, STANDARD, NORMALE);
		// Inversion de l'état de l'état du feu N-S
		myMemEtatFeux->nS = false;
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

		// Mise à jour des durées
		majDuree();
		afficherDuree();

		// Inversion de l'état du feux E-O
		myMemEtatFeux->eO = true;

		Afficher(COULEUR_AXE_EO, VERT, STANDARD, NORMALE);
		for(int i = dureeEO ; i > 0 ; i--)
		{
			afficherEO(i, DUREE_ARRET);
			sleep(1);
		}

		Afficher(COULEUR_AXE_EO, ORANGE, STANDARD, NORMALE);
		// Inversion de l'état de l'état du feux E-O
		myMemEtatFeux->eO = false;
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
		myMemEtatFeux->nS = true;
	}

} //----- fin de ActiverFeux

