/**
 * @file lobby.c
 * @brief Implémentation de la gestion du lobby.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../input.h"
#include "../socket.h"
#include "utils.h"
#include "lobby.h"

Lobby lobby = { 0 };

void initLobby()
{
	lobby.inLobby = true;
	lobby.nbBots = 0;
	lobby.nbJoueurs = 1;
	lobby.nbPrets = 0;
	bzero(lobby.joueurs, sizeof(lobby.joueurs));
}

void setInfoLobby(int botCount, int readyCount)
{
	lobby.nbBots = botCount;
	lobby.nbPrets = readyCount;
}

void printLobby()
{
	if (!lobby.inLobby)
	{
		return;
	}

	clear();
	printf("-- Lobby : en attente des autres joueurs --\n");

	printf("Nombre de joueurs prêts : %i / %i.\n", lobby.nbPrets, lobby.nbJoueurs);
	printf("Nombre de bots : %i.\n", lobby.nbBots);
	printf("\n");
	printf("Votre pseudo : %s.\n", joueur.nom);
	printf("Les autres joueurs :\n");
	for (int i = 0; i < lobby.nbJoueurs; i++) {
		if (i != joueur.id)
			printf("\t- %s\n", lobby.joueurs[i].nom);
	}
	printf("\n");
	if (joueur.ready)
	{
		printf("Vous êtes prêt.\n");
	}
	else
	{
		printfc(TERM_PURPLE, "Appuyer sur P pour devenir prêt.\n");
	}
	printfc(TERM_PURPLE, "Saisir un nombre pour changer le nombre de bots dans la partie.\n");
}

void gestionInputLobby()
{
	char* str = getUserInput();
	int nbBot = atoi(str);

	if (strcmp(str, "P") == 0 || strcmp(str, "p") == 0)
	{
		if ((lobby.nbBots > 0 || lobby.nbJoueurs > 1) && !joueur.ready)
		{
			joueur.ready = true;
			socket_send(CLI_MSG_SET_READY, NULL, 0);
		}
	}
	else {
		struct CliMsg_SetNumBot msgData = { .botCount = nbBot };
		socket_send(CLI_MSG_SET_NUM_BOT, &msgData, sizeof(msgData));
	}
}

void addPlayerToLobby(int id, const char* name)
{
	lobby.joueurs[id].id = id;
	lobby.joueurs[id].ready = false;
	strcpy(lobby.joueurs[id].nom, name);
	lobby.nbJoueurs++;
}


