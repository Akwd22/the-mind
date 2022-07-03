/**
 * @file handlers.c
 * @brief Implémentation des gestionnaires de messages reçus du serveur du fichier @link messaging/srv_handlers.h @endlink.
 */

#include <stdio.h>
#include "socket.h"
#include "logic.h"
#include "utils.h"
#include "messaging/srv_handlers.h"

 /// Inutilisé.
void SrvMsg_NoneHandler(void* data)
{
	// pDebug("[BOT] Message Handler: SRV_MSG_NONE - Unhandled message.\n");
}

/// Inutilisé.
void SrvMsg_InfoLobbyHandler(void* data)
{
	// pDebug("[BOT] Message Handler: SRV_INFO_LOBBY - Unhandled message.\n");
}

/// Inutilisé.
void SrvMsg_PlayerConnectedHandler(void* data)
{
	// pDebug("[BOT] Message Handler: SRV_PLAYER_CONNECTED - Unhandled message.\n");
}

/// Planifier le jouage du robot.
void SrvMsg_CardPlayedHandler(void* data)
{
	struct SrvMsg_CardPlayed* msg = (struct SrvMsg_CardPlayed*)data;
	pDebug("[BOT] Message Handler: SRV_MSG_CARD_PLAYED - Player id: %i - Card number: %i.\n", msg->playerId, msg->cardNumber);

	logic_schedulePlayCard();
}

/// Définir les nouvelles cartes du joueur robot et planifier le jouage du robot.
void SrvMsg_NextRoundHandler(void* data)
{
	struct SrvMsg_NextRound* msg = (struct SrvMsg_NextRound*)data;

	pDebug("[BOT] Message Handler: SRV_MSG_NEXT_ROUND - Round number: %i - Remaining life: %i - Is the last round won: %i - Player cards:", msg->roundNumber, msg->lifeRemaining, msg->isLastRoundWon);
	for (unsigned int i = 0; i < msg->roundNumber; i++) pDebug(" [%i] = %i", i, msg->playerCards[i]);
	pDebug(".\n");

	logic_nextRound();
	logic_setPlayerCards(msg->roundNumber, msg->playerCards);
	logic_schedulePlayCard();
}

/// Remise à zéro du joueur robot.
void SrvMsg_GameEndHandler(void* data)
{
	pDebug("[BOT] Message Handler: SRV_MSG_GAME_END.\n");

	logic_endGame();
}

/// Initialiser le joueur robot.
void SrvMsg_PlayerInfo(void* data)
{
	struct SrvMsg_PlayerInfo* msg = (struct SrvMsg_PlayerInfo*)data;
	pDebug("[BOT] Message Handler: SRV_MSG_PLAYER_INFO - Player id: %i.\n", msg->playerId);

	logic_initPlayer(msg->playerId);
}

/// Se déconnecter du serveur.
void SrvMsg_DisconnectAll(void* data)
{
	pDebug("Message Handler: SRV_MSG_DISCONNECT_ALL - Disconnected from the server.\n");

	socket_disconnect();
}

/// Inutilisé.
void SrvMsg_MaxHandler(void* data)
{
	// pDebug("[BOT] Message Handler: SRV_MSG_MAX - Unhandled message.\n");
}