/**
 * @file handlers.c
 * @brief Implémentation des gestionnaires de messages reçus du serveur du fichier @link messaging/srv_handlers.h @endlink.
 */

#include <stdio.h>
#include "logic/lobby.h"
#include "logic/partie.h"
#include "logic/joueur.h"
#include "input.h"
#include "utils.h"
#include "messaging/srv_handlers.h"

 /// Inutilisé.
void SrvMsg_NoneHandler(void* data)
{
	pDebug("Message Handler: SRV_MSG_NONE - Unhandled message.\n");
}

/// Actualiser l'affichage des informations du lobby.
void SrvMsg_InfoLobbyHandler(void* data)
{
	struct SrvMsg_InfoLobby* msg = (struct SrvMsg_InfoLobby*)data;
	pDebug("Message Handler: SRV_MSG_INFO_LOBBY - Bot count: %i - Ready count: %i.\n", msg->botCount, msg->readyCount);

	setInfoLobby(msg->botCount, msg->readyCount);
	printLobby();
}

/// Ajouter le joueur au lobby et actualiser l'affichage.
void SrvMsg_PlayerConnectedHandler(void* data)
{
	struct SrvMsg_PlayerConnected* msg = (struct SrvMsg_PlayerConnected*)data;
	pDebug("Message Handler: SRV_MSG_PLAYER_CONNECTED - Player id: %i - Player name: %s.\n", msg->playerId, msg->name);

	if (msg->playerId == (unsigned int)joueur.id)
	{
		return;
	}

	addPlayerToLobby(msg->playerId, msg->name);
	printLobby();
}

/// Afficher la carte posée et permettre au joueur de jouer une carte.
void SrvMsg_CardPlayedHandler(void* data)
{
	struct SrvMsg_CardPlayed* msg = (struct SrvMsg_CardPlayed*)data;
	pDebug("Message Handler: SRV_MSG_CARD_PLAYED - Player id: %i - Card number: %i.\n", msg->playerId, msg->cardNumber);

	setCartePose(msg->cardNumber, msg->playerId);
	printManche();
	setInputCallback(&gestionInputCarteJouer);
}

/// Définir les nouvelles cartes du joueur, et lui permettre de jouer une carte.
void SrvMsg_NextRoundHandler(void* data)
{
	struct SrvMsg_NextRound* msg = (struct SrvMsg_NextRound*)data;

	pDebug("Message Handler: SRV_MSG_NEXT_ROUND - Round number: %i - Remaining life: %i - Is the last round won: %i - Player cards:", msg->roundNumber, msg->lifeRemaining, msg->isLastRoundWon);
	for (unsigned int i = 0; i < msg->roundNumber; i++) pDebug(" [%i] = %i", i, msg->playerCards[i]);
	pDebug(".\n");

	lobby.inLobby = false;
	setNextRound(msg->roundNumber, msg->lifeRemaining, msg->isLastRoundWon);
	distribuerCartes((int*)msg->playerCards);
	printManche();
	setInputCallback(&gestionInputCarteJouer);
}

/// Ne plus permettre au joueur de jouer, et afficher l'écran de fin.
void SrvMsg_GameEndHandler(void* data)
{
	pDebug("Message Handler: SRV_MSG_GAME_END.\n");

	setInputCallback(&gestionInputFinPartie);
	finPartie();
}

/// Définir les informations du joueur, et lui permettre de configurer le lobby.
void SrvMsg_PlayerInfo(void* data)
{
	struct SrvMsg_PlayerInfo* msg = (struct SrvMsg_PlayerInfo*)data;
	pDebug("Message Handler: SRV_MSG_PLAYER_INFO - Player id: %i.\n", msg->playerId);

	setId(msg->playerId);
	initLobby();

	setInputCallback(&gestionInputLobby);
	printLobby();
}

/// Se déconnecter du serveur.
void SrvMsg_DisconnectAll(void* data)
{
	pDebug("Message Handler: SRV_MSG_DISCONNECT_ALL - Disconnected from the server.\n");

	setInputCallback(NULL);
	socket_disconnect();
	printfc(TERM_YELLOW, "Déconnecté du serveur. Vous pouvez fermer le jeu avec CTRL+C.\n");
}

/// Inutilisé.
void SrvMsg_MaxHandler(void* data)
{
	pDebug("Message Handler: SRV_MSG_MAX - Unhandled message.\n");
}