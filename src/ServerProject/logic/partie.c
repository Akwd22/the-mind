/**
 * @file partie.c
 * @brief Implémentation de la gestion d'une partie et des manches.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../socket.h"
#include "stats.h"
#include "partie.h"

Partie partie = { 0 };

/**
 * @brief Générer les cartes et les distribuer aux joueurs.
 */
void distribuerCartes()
{
	bool tmp[100] = { 0 };
	int carte = 0;
	for (int i = 0; i < partie.nbJoueurs; i++) {
		for (int j = 0; j < partie.manche; j++) {
			do {
				carte = rand() % (101 - 1) + 1;
				if (!tmp[carte]) {
					partie.joueurs[i].cartes[j] = carte;
					tmp[carte] = true;
					break;
				}
			} while (tmp[carte]);
		}
	}
}

/**
 * @brief Arrêter la partie et faire game over.
 */
void finPartie()
{
	stats_updateGameStats();
	socket_broadcast(SRV_MSG_GAME_END, NULL, 0);
}

/**
 * @brief Vérifier si la carte posée fait perdre ou continuer/gagner la manche.
 * @param numCarte Numéro de la carte (pas l'index).
 */
bool isCardWinner(int numCarte)
{
	if (partie.terrainJeu < numCarte) {
		return true;
	}
	return false;
}

/**
 * @brief Vérifier si tous les joueurs ont joués toutes leurs cartes.
 */
bool areAllCardsPlayed()
{
	bool allCardsPlayed = true;

	for (int i = 0; i < partie.nbJoueurs; i++)
	{
		for (int j = 0; j < partie.manche; j++)
		{
			if (partie.joueurs[i].cartes[j] != 0)
			{
				allCardsPlayed = false;
				break;
			}
		}
	}

	return allCardsPlayed;
}

/**
 * @brief Faire perdre la manche et passer à la prochaine si possible. Retirer une vie, ou game over.
 */
void manchePerdu()
{
	partie.vie = partie.vie - 1;

	if (partie.vie <= 0) {
		finPartie();
		return;
	}

	partie.terrainJeu = 0;
	distribuerCartes();

	for (int i = 0; i < partie.nbJoueurs; i++) {
		struct SrvMsg_NextRound msgData = { .roundNumber = partie.manche, .lifeRemaining = partie.vie, .isLastRoundWon = false };
		memcpy(msgData.playerCards, partie.joueurs[i].cartes, partie.manche * sizeof(int));
		socket_send(i, SRV_MSG_NEXT_ROUND, &msgData, sizeof(msgData));
	}
}

/**
 * @brief Passer à la prochaine manche.
 */
void mancheGagner()
{
	partie.manche = partie.manche + 1;
	partie.terrainJeu = 0;
	distribuerCartes();

	for (int i = 0; i < partie.nbJoueurs; i++) {
		struct SrvMsg_NextRound msgData = { .roundNumber = partie.manche, .lifeRemaining = partie.vie, .isLastRoundWon = true };
		memcpy(msgData.playerCards, partie.joueurs[i].cartes, partie.manche * sizeof(int));
		socket_send(i, SRV_MSG_NEXT_ROUND, &msgData, sizeof(msgData));
	}
}

bool gestionCarteJouer(int idJoueur, int idCarte)
{
	// Mettre à jour les statistiques.
	unsigned int reactionTime = stats_elapsedSecs(false);
	stats_updatePlayerReactionTimes(idJoueur, reactionTime);
	stats_updateReactionTimes(reactionTime);
	stats_elapsedSecs(true);

	// Gérer la carte.
	int numCarte = partie.joueurs[idJoueur].cartes[idCarte];

	partie.joueurs[idJoueur].cartes[idCarte] = 0; // Retirer la carte de la main du joueur.

	// La carte posée fait gagner la manche et toutes les cartes ont été posées.
	if (isCardWinner(numCarte) && areAllCardsPlayed()) {
		mancheGagner();
		return false;
	}

	// La carte posée fait perdre la manche.
	if (!isCardWinner(numCarte)) {
		stats_updatePlayerFailCount(idJoueur);
		manchePerdu();
		return false;
	}

	// Continuer la manche.
	partie.terrainJeu = numCarte; // Poser la carte.
	return true;
}

void initPartie(Joueur tab[], int n)
{
	partie.nbJoueurs = n;
	partie.manche = 0;
	partie.vie = 3;

	for (int i = 0; i < n; i++) {
		partie.joueurs[i] = tab[i];
	}

	mancheGagner(); // Débuter le 1ère manche.
}