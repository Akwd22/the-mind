/**
 * @file logic.c
 * @brief Implémentation de la gestion du jeu en lui-même et de la partie.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "socket.h"
#include "utils.h"
#include "logic.h"

 /// Instance du joueur robot.
Player player = { 0 };

/// Afficher les cartes non jouées du robot.
void logic_printCards()
{
	if (player.nbPlayedCards == player.nbCards) return;

	printfc(TERM_PURPLE, "%s cards :", player.playerName);

	for (unsigned int i = 0; i < player.nbCards; i++)
	{
		unsigned int cardNumber = player.cards[i];

		// N'afficher que les cartes non jouées.
		if (cardNumber > 0)
		{
			printfc(TERM_PURPLE, " %i", cardNumber);
		}
	}

	printfc(TERM_PURPLE, ".\n");
}

/// Retourner l'index de la plus petite carte non jouée du robot.
unsigned int logic_getLowestCardIndex()
{
	unsigned int lowestCardIndex = 0;
	unsigned int lowestCardNumber = player.cards[lowestCardIndex];

	for (unsigned int i = 0; i < player.nbCards; i++)
	{
		unsigned int cardNumber = player.cards[i];

		if (!cardNumber) continue; // Carte déjà jouée, on passe.

		if (!lowestCardNumber || cardNumber < lowestCardNumber)
		{
			lowestCardIndex = i;
			lowestCardNumber = cardNumber;
		}
	}

	return lowestCardIndex;
}

void logic_initPlayer(unsigned int playerId)
{
	player.playerId = playerId;
	sprintf(player.playerName, "Bot %c", 'A' + playerId);
	player.nbCards = 0;
	player.nbPlayedCards = 0;
	player.cards = NULL;
	player.canPlay = false;
}

void logic_endGame()
{
	free(player.cards);
	logic_initPlayer(player.playerId);
}

void logic_nextRound()
{
	player.canPlay = true;
}

void logic_removePlayerCard(unsigned int cardIndex)
{
	player.cards[cardIndex] = 0;
	player.nbPlayedCards++;

	// Si le joueur a joué toutes ses cartes, il ne peut plus jouer.
	if (player.nbPlayedCards == player.nbCards)
	{
		player.canPlay = false;
	}
}

void logic_setPlayerCards(unsigned int nbCards, const unsigned int* cards)
{
	free(player.cards);

	player.nbCards = nbCards;
	player.nbPlayedCards = 0;
	player.cards = (unsigned int*)calloc(nbCards, sizeof(unsigned int));

	if (!player.cards)
	{
		FATAL_ERR("setPlayerCards - calloc()");
	}

	memcpy(player.cards, cards, nbCards * sizeof(unsigned int));

	logic_printCards();
}

void logic_playCard()
{
	if (!player.canPlay)
	{
		return;
	}

	unsigned int cardIndex = logic_getLowestCardIndex();
	unsigned int cardNumber = player.cards[cardIndex];

	struct CliMsg_PlayCard msgData = { .cardIndex = cardIndex };
	socket_send(CLI_MSG_PLAY_CARD, &msgData, sizeof(msgData));

	logic_removePlayerCard(cardIndex);

	printfc(TERM_PURPLE, "%s played card number %i.\n", player.playerName, cardNumber);
	logic_printCards();
}

void logic_schedulePlayCard()
{
	if (!player.canPlay)
	{
		alarm(0);
		return;
	}

	signal(SIGALRM, &logic_playCard);

	unsigned int delayToPlay = rand() % (MAX_PLAYING_INTERVAL - MIN_PLAYING_INTERVAL + 1) + MIN_PLAYING_INTERVAL;
	alarm(delayToPlay);

	printfc(TERM_PURPLE, "%s will play a card in %i seconds if no one plays before.\n", player.playerName, delayToPlay);
}