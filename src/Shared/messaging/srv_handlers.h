/**
 * @file srv_handlers.h
 * @brief Définitions des gestionnaires des types de message envoyé par le serveur.
 *
 * Les gestionnaires doivent être implémentés au niveau du client.
 * Les gestionnaires sont automatiquement appelés à la réception d'un message.
 */

#pragma once

#include "enums.h"
#include "structs.h"

void SrvMsg_NoneHandler(void* data);
void SrvMsg_InfoLobbyHandler(void* data);
void SrvMsg_PlayerConnectedHandler(void* data);
void SrvMsg_CardPlayedHandler(void* data);
void SrvMsg_NextRoundHandler(void* data);
void SrvMsg_GameEndHandler(void* data);
void SrvMsg_PlayerInfo(void* data);
void SrvMsg_DisconnectAll(void* data);
void SrvMsg_MaxHandler(void* data);

/**
 * @brief Tableau associatif (type du message serveur => fonction qui gère le message).
 * Les fonctions gestionnaires ont un paramètre : pointeur vers la structure correspondant au type du message (cf. @link messaging/structs.h @endlink).
 * @see Voir l'énumération @link SrvMsg @endlink pour les types de message client.
*/
static void (*srvMsgHandler[SRV_MSG_MAX + 1])(void* data) = {
	[0] = &SrvMsg_NoneHandler,[SRV_MSG_MAX] = &SrvMsg_MaxHandler,
	[1] = &SrvMsg_InfoLobbyHandler,
	[2] = &SrvMsg_PlayerConnectedHandler,
	[3] = &SrvMsg_CardPlayedHandler,
	[4] = &SrvMsg_NextRoundHandler,
	[5] = &SrvMsg_GameEndHandler,
	[6] = &SrvMsg_PlayerInfo,
	[7] = &SrvMsg_DisconnectAll
};