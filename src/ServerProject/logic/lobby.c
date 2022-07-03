/**
 * @file lobby.c
 * @brief Implémentation de la gestion du lobby.
 */

#include <stdlib.h>
#include "partie.h"
#include "stats.h"
#include "lobby.h"

Lobby lobby = { 0 };

void startGame()
{
	stats_elapsedSecs(true);
	initPartie(lobby.joueurs, lobby.nbJoueurs);
}

void setNumBot(int nb)
{
	if (nb < 0)
		return;
	lobby.nbBots = nb;
}

void addPlayerToLobby(int id)
{
	lobby.joueurs[id].id = id;
	lobby.joueurs[id].ready = false;
	lobby.nbJoueurs++;
}


