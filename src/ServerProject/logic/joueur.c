/**
 * @file joueur.c
 * @brief Implémentation de la gestion d'un joueur.
 */

#include <string.h>
#include "lobby.h"
#include "stats.h"
#include "joueur.h"

void setPlayerName(int id, const char* name)
{
	strcpy(lobby.joueurs[id].nom, name);
}

void setReady(int id)
{
	lobby.joueurs[id].ready = true;
	lobby.nbPrets++;
}