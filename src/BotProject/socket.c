/**
 * @file socket.c
 * @brief Implémentation de la gestion de la connexion socket au serveur.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "socket/consts.h"
#include "socket/utils_io.h"
#include "messaging/srv_handlers.h"
#include "utils.h"
#include "socket.h"

 /// Adresse IP du serveur.
char* serverIp;
/// Port du serveur.
unsigned short serverPort;

/// Descripteur du socket.
SOCKET cliSocket;
/// Socket est-il ouvert ?
bool isOpened = false;

void socket_setIp(char* ip)
{
	serverIp = ip;
}

void socket_setPort(unsigned short port)
{
	serverPort = port;
}

/// Écouter les messages du serveur et appeler le bon gestionnaire selon le message reçu.
void listenMessages()
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

		pDebug("[BOT] Message received (type %i) (%lu bytes).\n", header.msgType, header.dataLen + sizeof(header));

		// Appel du gestionnaire correspondant au type du message.
		srvMsgHandler[header.msgType](data);
	}

	pDebug("[BOT] Stopped communication with the server.\n");
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

	pDebug("[BOT] Sent message (type %i) (%li bytes).\n", header.msgType, header.dataLen + sizeof(header));
}

void socket_connect()
{
	pDebug("[BOT] Connecting to the server...\n");

	// Création du socket.
	cliSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (cliSocket == -1)
	{
		FATAL_ERR("socket_connect - socket()");
	}

	// Spécifier l'adresse IP et le port du serveur.
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(serverPort);

	if (inet_aton(serverIp, &sin.sin_addr) == -1)
	{
		FATAL_ERR("socket_connect - inet_aton()");
	}

	// Connexion au serveur.
	if (connect(cliSocket, (struct sockaddr*)&sin, sizeof sin) == -1)
	{
		FATAL_ERR("socket_connect - connect()");
	}

	isOpened = true;

	pDebug("[BOT] Connected to the server.\n");

	// Avertir le serveur de la connexion.
	socket_send(CLI_MSG_BOT_CONNECT, NULL, 0);

	// Écouter les messages.
	listenMessages();

	while (isOpened);
}

void socket_disconnect()
{
	isOpened = false;
	close(cliSocket);

	pDebug("[BOT] Disconnected from the server.\n");
}