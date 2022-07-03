/**
 * @file lobby.h
 * @brief Interface permettant de gérer le lobby.
 */

#pragma once

#include "../socket.h"
#include "socket/consts.h"
#include "joueur.h"

 /// Structure stockant les données du lobby.
typedef struct S_Lobby {
	/// Nombre de robots présents.
	int nbBots;
	/// Nombre de joueurs prêts pour démarrer la partie.
	int nbPrets;
	/// Nombre de joueurs dans le lobby.
	int nbJoueurs;
	/// Tableau contenant les joueurs (index du tableau = identifiant du joueur).
	Joueur joueurs[MAX_CONNECTIONS];
}Lobby;

/// Instance du lobby.
extern Lobby lobby;

/**
 * @brief Démarrer la partie et réinitialiser les données de l'ancienne partie si ce n'est pas la première.
 */
void startGame();

/**
 * @brief Ajouter un joueur au lobby.
 * @param id Identifiant du joueur.
 */
void addPlayerToLobby(int id);

/**
 * @brief Définir le nombre de robots.
 * @param nb Nouveau nombre.
 */
void setNumBot(int nb);