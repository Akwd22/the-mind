/**
 * @file lobby.h
 * @brief Interface permettant la gestion du lobby.
 */

#pragma once

#include "joueur.h"
#include "../socket.h"
#include "socket/consts.h"

 /// Structure stockant les données du lobby.
typedef struct S_Lobby {
	/// Est-on dans le lobby ?
	bool inLobby;
	/// Nombre de robots présents.
	int nbBots;
	/// Nombre de joueurs prêts.
	int nbPrets;
	/// Nombre de joueurs.
	int nbJoueurs;
	/// Tableau contenant les joueurs (index du tableau = identifiant du joueur).
	Joueur joueurs[MAX_CONNECTIONS];
}Lobby;

/// Instance du lobby.
extern Lobby lobby;

/**
 * @brief Initialiser le lobby.
 */
void initLobby();

/**
 * @brief Ajouter un joueur au lobby.
 * @param id Identifiant du joueur.
 * @param name Nom du joueur.
 */
void addPlayerToLobby(int id, const char* name);

/**
 * @brief Définir les informations du lobby.
 * @param botCount Nombre de robots présents.
 * @param readyCount Nombre de joueurs prêts.
 */
void setInfoLobby(int botCount, int readyCount);

/**
 * @brief Afficher le lobby.
 */
void printLobby();

/**
 * @brief Gérer les saisies utilisateurs dans le lobby.
 * Permettre de devenir prêt et de définir le nombre de robots.
 */
void gestionInputLobby();