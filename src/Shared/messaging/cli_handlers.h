/**
 * @file cli_handlers.h
 * @brief Définitions des gestionnaires des types de message envoyé par le client.
 *
 * Les gestionnaires doivent être implémentés au niveau du serveur.
 * Les gestionnaires sont automatiquement appelés à la réception d'un message.
 */

#pragma once

#include "enums.h"
#include "structs.h"

void CliMsg_NoneHandler(unsigned int senderId, void* data);
void CliMsg_SetNameHandler(unsigned int senderId, void* data);
void CliMsg_SetReadyHandler(unsigned int senderId, void* data);
void CliMsg_SetNumBotHandler(unsigned int senderId, void* data);
void CliMsg_PlayCardHandler(unsigned int senderId, void* data);
void CliMsg_ReplayGameHandler(unsigned int senderId, void* data);
void CliMsg_BotConnectHandler(unsigned int senderId, void* data);
void CliMsg_StopGameHandler(unsigned int senderId, void* data);
void CliMsg_MaxHandler(unsigned int senderId, void* data);

/**
 * @brief Tableau associatif (type du message client => fonction qui gère le message).
 * Les fonctions gestionnaires ont 2 paramètres :
 * - identifiant du client émetteur ;
 * - pointeur vers la structure correspondant au type du message (cf. @link messaging/structs.h @endlink).
 * @see Voir l'énumération @link CliMsg @endlink pour les types de message client.
*/
static void (*cliMsgHandler[CLI_MSG_MAX + 1])(unsigned int, void*) = {
	[0] = &CliMsg_NoneHandler,[CLI_MSG_MAX] = &CliMsg_MaxHandler,
	[1] = &CliMsg_SetNameHandler,
	[2] = &CliMsg_SetReadyHandler,
	[3] = &CliMsg_SetNumBotHandler,
	[4] = &CliMsg_PlayCardHandler,
	[5] = &CliMsg_ReplayGameHandler,
	[6] = &CliMsg_BotConnectHandler,
	[7] = &CliMsg_StopGameHandler
};