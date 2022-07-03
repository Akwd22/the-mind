/**
 * @file socket.c
 * @brief Implémentation de la gestion de la connexion socket au serveur.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include "socket/consts.h"
#include "socket/utils_io.h"
#include "messaging/srv_handlers.h"
#include "logic/joueur.h"
#include "utils.h"
#include "socket.h"

 /// Descripteur du socket.
SOCKET cliSocket;
/// Socket est-il ouvert ?
bool isOpened = false;

/**
 * @brief [Thread] Écouter les messages du serveur et appeler le bon gestionnaire selon le message reçu.
*/
void* listenMessages(void* unused)
{
	while (isOpened)
	{
		struct MsgHeader header;
		char data[MAX_MSG_SIZE];

		// Lecture en-tête message : taille contenu du message & type du message.
		if (recvn(cliSocket, &header, sizeof(header)) == -1)
		{
			FATAL_ERR("listenMessages - header - recvn()");
		};

		// Lecture contenu message.
		if (recvn(cliSocket, data, header.dataLen) == -1)
		{
			FATAL_ERR("listenMessages - data - recvn()");
		};

		pDebug("%sMessage received (type %i) (%lu bytes).\n", TERM_BLUE, header.msgType, header.dataLen + sizeof(header));

		// Appel du gestionnaire correspondant au type du message.
		srvMsgHandler[header.msgType](data);
	}

	pDebug("Stopped communication with the server.\n");
}

void socket_send(enum CliMsg type, const void* msg, size_t size)
{
	struct MsgHeader header = { size, type };

	// Envoyer l'en-tête du message.
	if (sendn(cliSocket, &header, sizeof(header)) == -1)
	{
		FATAL_ERR("socket_send - header - sendn()");
	};

	// Envoyer le contenu du message.
	if (sendn(cliSocket, msg, size) == -1)
	{
		FATAL_ERR("socket_send - data - sendn()");
	};

	pDebug("%sSent message (type %i) (%li bytes).\n", TERM_GREEN, header.msgType, header.dataLen + sizeof(header));
}

bool socket_connect(const char* ip, unsigned short port)
{
	pDebug("Connecting to the server...\n");

	// Création du socket.
	cliSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (cliSocket == -1)
	{
		FATAL_ERR("socket_connect - socket()");
	}

	// Spécifier l'adresse IP et le port du serveur.
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	if (inet_aton(ip, &sin.sin_addr) == -1)
	{
		FATAL_ERR("socket_connect - inet_aton()");
	}

	// Connexion au serveur.
	if (connect(cliSocket, (struct sockaddr*)&sin, sizeof sin) == -1)
	{
		fprintf(stderr, "Erreur de connexion au serveur : %s.\nRéessayez dans 5 secondes...\n", strerror(errno));
		sleep(5);
		return false;
	}

	isOpened = true;

	pDebug("Connected to the server.\n");

	// Démarrer l'écoute des messages reçus.
	pthread_t threadId;
	if (pthread_create(&threadId, NULL, &listenMessages, NULL) == -1)
	{
		FATAL_ERR("listenMessages - pthread_create()");
	}

	// Envoyer le nom du joueur au serveur.
	struct CliMsg_SetName msgData = { 0 };
	strcpy(msgData.name, joueur.nom);
	socket_send(CLI_MSG_SET_NAME, &msgData, sizeof(msgData));

	return true;
}

void socket_disconnect()
{
	isOpened = false;
	close(cliSocket);

	pDebug("Disconnected from the server.\n");
}