/**
 * @file lobby.c
 * @brief Implémentation de la gestion de la partie et des manches.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../input.h"
#include "../socket.h"
#include "lobby.h"
#include "joueur.h"
#include "utils.h"
#include "partie.h"

 /// Instance de la partie.
Partie partie = { 0 };

void distribuerCartes(const int* playercard)
{
	joueur.nbCartes = partie.manche;
	bzero(joueur.cartes, (joueur.nbCartes - 1) * sizeof(int));
	memcpy(joueur.cartes, playercard, joueur.nbCartes * sizeof(int));
}

void setCartePose(int carte, int idJCarte) {
	lobby.joueurs[idJCarte].nbCartes--;
	partie.cartePose = carte;
	partie.idJCartePose = idJCarte;
}

void setNextRound(int roundNumber, int lifeRemaining, int isLastRoundWon)
{
	partie.manche = roundNumber;
	partie.vie = lifeRemaining;

	partie.cartePose = 0;
	partie.idJCartePose = 0;

	partie.lastRoundWon = isLastRoundWon;

	for (int i = 0; i < lobby.nbJoueurs; i++)
	{
		lobby.joueurs[i].nbCartes = partie.manche;
	}
}

bool areAllCardsPlayed()
{
	bool areAllPlayed = true;

	for (int i = 0; i < joueur.nbCartes; i++)
	{
		if (joueur.cartes[i] != 0)
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

	printf("Nombre de manches gagnées : %i.\n", partie.manche - 1);

	printf("\n\n");

	printfc(TERM_PURPLE, "Saisir la touche R pour recommencer une partie.\n");
	printfc(TERM_PURPLE, "Saisir la touche Q pour arrêter le jeu.\n");
}

void printManche()
{
	if (lobby.inLobby)
	{
		return;
	}

	clear();

	printf("-- Partie en cours --\n");

	printf("Numéro de la manche : %i.\n", partie.manche);
	printf("Nombre restant de vie(s) : %i.\n", partie.vie);

	if (partie.lastRoundWon)
	{
		printfc(TERM_GREEN, "La manche précédente a été gagnée.\n");
	}
	else
	{
		printfc(TERM_RED, "La manche précédente a été perdue.\n");
	}

	printf("\n");

	if (!partie.cartePose)
	{
		printfc(TERM_YELLOW, "-> Aucune carte n'a été jouée.\n");
	}
	else
	{
		printfc(TERM_YELLOW, "-> Carte posée par le joueur %s : %i.\n", partie.idJCartePose == joueur.id ? joueur.nom : lobby.joueurs[partie.idJCartePose].nom, partie.cartePose);
	}

	printf("\n");

	printf("Nombre restant de cartes des autres joueurs :\n");

	for (int i = 0; i < lobby.nbJoueurs; i++)
	{
		if (i == joueur.id) continue;

		int nbCartes = lobby.joueurs[i].nbCartes;

		if (nbCartes == 0)
		{
			printfc(TERM_GREY, "\t- %s : %i / %i\n", lobby.joueurs[i].nom, nbCartes, partie.manche);
		}
		else
		{
			printfc(TERM_DEFAULT, "\t- %s : %i / %i\n", lobby.joueurs[i].nom, nbCartes, partie.manche);
		}
	}

	printf("\n\n");

	if (areAllCardsPlayed())
	{
		printf("Vous avez joué toutes vos cartes.\n");
	}
	else
	{
		printf("Votre main :");

		for (int i = 0; i < partie.manche; i++)
		{
			int numCarte = joueur.cartes[i];

			if (numCarte > 0)
			{
				printf(" %i", numCarte);
			}
		}

		printf("\n");

		printfc(TERM_PURPLE, "Saisir le numéro de la carte que vous voulez poser.\n");
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
	else if (strcmp(input, "Q") == 0 || strcmp(input, "q") == 0)
	{
		socket_send(CLI_MSG_STOP_GAME, NULL, 0);
	}
}