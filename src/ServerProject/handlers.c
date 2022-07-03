/**
 * @file handlers.c
 * @brief Implémentation des gestionnaires de messages reçus des clients du fichier @link messaging/cli_handlers.h @endlink.
 */

#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "logic/partie.h"
#include "logic/lobby.h"
#include "logic/stats.h"
#include "messaging/cli_handlers.h"

#include "utils.h"

 /// Inutilisé.
void CliMsg_NoneHandler(unsigned int senderId, void* data)
{
	printf("Message Handler: CLI_MSG_NONE - Client: %i - Unhandled message.\n", senderId);
}

/// Définir le nom du joueur.
void CliMsg_SetNameHandler(unsigned int senderId, void* data)
{
	struct CliMsg_SetName* msg = (struct CliMsg_SetName*)data;
	printf("Message Handler: CLI_MSG_SET_NAME - Client: %i - New name: %s.\n", senderId, msg->name);

	setPlayerName(senderId, msg->name);
	addPlayerToLobby(senderId);

	struct SrvMsg_PlayerInfo msgData1 = { .playerId = senderId };
	socket_send(senderId, SRV_MSG_PLAYER_INFO, &msgData1, sizeof(msgData1));

	struct SrvMsg_PlayerConnected msgData2 = { 0 };
	strcpy(msgData2.name, msg->name);
	msgData2.playerId = senderId;
	msgData2.isBot = false;
	socket_broadcast(SRV_MSG_PLAYER_CONNECTED, &msgData2, sizeof(msgData2));

	for (int i = 0; i < lobby.nbJoueurs; i++) {
		struct SrvMsg_PlayerConnected msgData3 = { 0 };
		strcpy(msgData3.name, lobby.joueurs[i].nom);
		msgData3.playerId = i;
		msgData3.isBot = false;
		socket_send(senderId, SRV_MSG_PLAYER_CONNECTED, &msgData3, sizeof(msgData3));

		struct SrvMsg_InfoLobby msgData4 = { .botCount = lobby.nbBots, .readyCount = lobby.nbPrets };
		socket_send(senderId, SRV_MSG_INFO_LOBBY, &msgData4, sizeof(msgData4));
	}
}

/// Incrémenter le nombre de joueurs prêts et démarrer la partie si tous les joueurs sont prêts.
void CliMsg_SetReadyHandler(unsigned int senderId, void* data)
{
	printf("Message Handler: CLI_MSG_SET_READY - Client: %i - Is now ready to play.\n", senderId);

	// Le joueur est tout seul dans le lobby.
	if (lobby.nbJoueurs == 1 && lobby.nbBots == 0) return;

	setReady(senderId);

	// Avertir les autres clients qu'un joueur est prêt.
	{
		struct SrvMsg_InfoLobby msgData = { .botCount = lobby.nbBots, .readyCount = lobby.nbPrets };
		socket_broadcast(SRV_MSG_INFO_LOBBY, &msgData, sizeof(msgData));
	}

	// Si tout le monde est prêt, connecter les robots et lancer la partie.
	if (lobby.nbPrets == lobby.nbJoueurs)
	{
		lobby.nbBots == 0 ? startGame() : socket_bots(lobby.nbBots);
	}
}

/// Définir le nouveau nombre de robots dans la partie.
void CliMsg_SetNumBotHandler(unsigned int senderId, void* data)
{
	struct CliMsg_SetNumBot* msg = (struct CliMsg_SetNumBot*)data;
	printf("Message Handler: CLI_MSG_SET_NUM_BOT - Client: %i - New bot count: %i.\n", senderId, msg->botCount);

	setNumBot(msg->botCount);

	struct SrvMsg_InfoLobby msgData = { .botCount = lobby.nbBots, .readyCount = lobby.nbPrets };
	socket_broadcast(SRV_MSG_INFO_LOBBY, &msgData, sizeof(msgData));
}

/// Vérifier si la carte fait gagner, perdre, ou continuer la manche. Effectuer l'action d'un de ses trois cas.
void CliMsg_PlayCardHandler(unsigned int senderId, void* data)
{
	struct CliMsg_PlayCard* msg = (struct CliMsg_PlayCard*)data;
	printf("Message Handler: CLI_MSG_PLAY_CARD - Client: %i - Play card index: %i.\n", senderId, msg->cardIndex);

	int numCarte = partie.joueurs[senderId].cartes[msg->cardIndex];
	bool continuer = gestionCarteJouer(senderId, msg->cardIndex);

	if (continuer) {
		struct SrvMsg_CardPlayed msgData = { .playerId = senderId, .cardNumber = numCarte };
		socket_broadcast(SRV_MSG_CARD_PLAYED, &msgData, sizeof(msgData));
	}
}

/// Relancer une partie.
void CliMsg_ReplayGameHandler(unsigned int senderId, void* data)
{
	printf("Message Handler: CLI_MSG_REPLAY_GAME - Client: %i - Starting a new game.\n", senderId);

	startGame();
}

/// Ajouter le robot aux joueurs et démarrer la partie lorsque tous les robots sont connectés.
void CliMsg_BotConnectHandler(unsigned int senderId, void* data)
{
	printf("Message Handler: CLI_MSG_BOT_CONNECT - Bot: %i - Connected to the server.\n", senderId);

	static unsigned int nbConnectedBots = 0;

	// Nom du robot.
	char name[20] = ""; sprintf(name, "Robot %c", 'A' + senderId);

	// Ajouter le bot à la liste des joueurs.
	addPlayerToLobby(senderId);
	setPlayerName(senderId, name);
	nbConnectedBots++;

	// Envoyer à l'émetteur ses informations client.
	{
		struct SrvMsg_PlayerInfo msgData = { .playerId = senderId };
		socket_send(senderId, SRV_MSG_PLAYER_INFO, &msgData, sizeof(msgData));
	}

	// Avertir les autres clients de la connexion du robot.
	{
		struct SrvMsg_PlayerConnected msgData = { .playerId = senderId, .isBot = true, .name = "" };
		strcpy(msgData.name, name);

		socket_broadcast(SRV_MSG_PLAYER_CONNECTED, &msgData, sizeof(msgData));
	}

	// Lancer la partie si tous les robots sont connectés.
	if (nbConnectedBots == (unsigned int)lobby.nbBots)
	{
		startGame();
	}
}

/// Générer le PDF des statistiques et fermer les sockets.
void CliMsg_StopGameHandler(unsigned int senderId, void* data)
{
	printf("Message Handler: CLI_MSG_STOP_GAME - Client: %i - Stopping the game.\n", senderId);

	stats_generatePDF();
	socket_requestClose(); // TODO : main thread ?
}

/// Inutilisé.
void CliMsg_MaxHandler(unsigned int senderId, void* data)
{
	printf("Message Handler: CLI_MSG_MAX - Client: %i - Unhandled message.\n", senderId);
}