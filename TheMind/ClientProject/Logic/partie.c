#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "../input.h"
#include "messaging/structs.h"
#include "messaging/enums.h"
#include "../socket.h"
#include "partie.h"
#include "lobby.h"
#include "joueur.h"
#include "utils.h"

partie p = { 0 };

void distribuerCartes(int* playercard)
{
	j.nbCartes = p.manche;
	bzero(j.cartes, (j.nbCartes - 1) * sizeof(int));
	memcpy(j.cartes, playercard, j.nbCartes * sizeof(int));
}

void setCartePose(int carte, int idJCarte) {
	l.joueurs[idJCarte].nbCartes--;
	p.cartePose = carte;
	p.idJCartePose = idJCarte;
}

void setNextRound(int roundNumber, int lifeRemaining, int isLastRoundWon)
{
	p.manche = roundNumber;
	p.vie = lifeRemaining;

	p.cartePose = 0;
	p.idJCartePose = 0;

	p.lastRoundWon = isLastRoundWon;

	for (int i = 0; i < l.nbJoueurs; i++)
	{
		l.joueurs[i].nbCartes = p.manche;
	}
}

bool areAllCardsPlayed()
{
	bool areAllPlayed = true;

	for (int i = 0; i < j.nbCartes; i++)
	{
		if (j.cartes[i] != 0)
		{
			areAllPlayed = false;
			break;
		}
	}

	return areAllPlayed;
}

void finPartie()
{
	clear();
	printf("-- Fin de partie --\n");

	printf("Game Over !\n");
	printf("Vous avez perdu toutes vos vies.\n");

	printf("\n");

	printf("Nombre de manches gagn�es : %i.\n", p.manche - 1);

	printf("\n\n");

	printfc(TERM_PURPLE, "Saisir la touche R pour recommencer une partie.\n");
}

void printManche()
{
	if (l.inLobby)
	{
		return;
	}

	clear();

	printf("-- Partie en cours --\n");

	printf("Num�ro de la manche : %i.\n", p.manche);
	printf("Nombre restant de vie(s) : %i.\n", p.vie);

	if (p.lastRoundWon)
	{
		printfc(TERM_GREEN, "La manche pr�c�dente a �t� gagn�e.\n");
	}
	else
	{
		printfc(TERM_RED, "La manche pr�c�dente a �t� perdue.\n");
	}

	printf("\n");

	if (!p.cartePose)
	{
		printfc(TERM_YELLOW, "-> Aucune carte n'a �t� jou�e.\n");
	}
	else
	{
		printfc(TERM_YELLOW, "-> Carte pos�e par le joueur %s : %i.\n", p.idJCartePose == j.id ? j.nom : l.joueurs[p.idJCartePose].nom, p.cartePose);
	}

	printf("\n");

	printf("Nombre restant de cartes des autres joueurs :\n");

	for (int i = 0; i < l.nbJoueurs; i++)
	{
		if (i == j.id) continue;

		int nbCartes = l.joueurs[i].nbCartes;

		if (nbCartes == 0)
		{
			printfc(TERM_GREY, "\t- %s : %i / %i\n", l.joueurs[i].nom, nbCartes, p.manche);
		}
		else
		{
			printfc(TERM_DEFAULT, "\t- %s : %i / %i\n", l.joueurs[i].nom, nbCartes, p.manche);
		}
	}

	printf("\n\n");

	if (areAllCardsPlayed())
	{
		printf("Vous avez jou� toutes vos cartes.\n");
	}
	else
	{
		printf("Votre main :");

		for (int i = 0; i < p.manche; i++)
		{
			int numCarte = j.cartes[i];

			if (numCarte > 0)
			{
				printf(" %i", numCarte);
			}
		}

		printf("\n");

		printfc(TERM_PURPLE, "Saisir le num�ro de la carte que vous voulez poser.\n");
	}
}

void gestionInputCarteJouer()
{
	char* input = getUserInput();
	int numCarte = atoi(input);

	if (numCarte <= 0)
	{
		return;
	}

	jouerCarte(numCarte);
}

void gestionInputFinPartie()
{
	char* input = getUserInput();

	if (strcmp(input, "R") == 0 || strcmp(input, "r") == 0)
	{
		socket_send(CLI_MSG_REPLAY_GAME, NULL, 0);
	}
}