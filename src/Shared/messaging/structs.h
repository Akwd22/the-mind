/**
 * @file structs.h
 * @brief Structures représentant les données de chaque type de message.
 *
 * Un message possède une en-tête @link MsgHeader @endlink et certains types de message, un corps contenant les données utiles @link CliMsg_<...> @endlink et @link SrvMsg_<...> @endlink.
 */

#pragma once

 /// Structure de l'en-tête d'un message.
struct MsgHeader
{
	/// Longueur du contenu du message.
	unsigned int dataLen;
	/// Type du message (cf. énumérations @link CliMsg @endlink ou @link SrvMsg @endlink).
	unsigned int msgType;
};

/// Données du type de message @link CLI_MSG_SET_NAME @endlink.
struct CliMsg_SetName
{
	char name[20];
};

/// Données du type de message @link CLI_MSG_SET_NUM_BOT @endlink.
struct CliMsg_SetNumBot
{
	unsigned int botCount;
};

/// Données du type de message @link CLI_MSG_PLAY_CARD @endlink.
struct CliMsg_PlayCard
{
	unsigned int cardIndex;
};

/// Données du type de message @link SRV_MSG_INFO_LOBBY @endlink.
struct SrvMsg_InfoLobby
{
	/// Nombre de robots dans la partie.
	unsigned int botCount;
	/// Nombre de clients prêts à jouer.
	unsigned int readyCount;
};

/// Données du type de message @link SRV_MSG_PLAYER_CONNECTED @endlink.
struct SrvMsg_PlayerConnected
{
	unsigned int playerId;
	unsigned int isBot;
	char name[20];
};

/// Données du type de message @link SRV_MSG_CARD_PLAYED @endlink.
struct SrvMsg_CardPlayed
{
	unsigned int playerId;
	unsigned int cardNumber;
};

/// Données du type de message @link SRV_MSG_NEXT_ROUND @endlink.
struct SrvMsg_NextRound
{
	/// Numéro de la nouvelle manche.
	unsigned int roundNumber;
	/// Nombre restant de vie.
	unsigned int lifeRemaining;
	/// La dernière manche a été gagnée ?
	unsigned int isLastRoundWon;
	/// Ensemble des cartes du client destinataire du message.
	unsigned int playerCards[50];
};

/// Données du type de message @link SRV_MSG_PLAYER_INFO @endlink.
struct SrvMsg_PlayerInfo
{
	unsigned int playerId;
};