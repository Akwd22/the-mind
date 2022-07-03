/**
 * @file partie.h
 * @brief Interface permettant de gérer la partie et les manches.
 */

#pragma once

#include <stdbool.h>
#include "socket/consts.h"
#include "joueur.h"

 /// Structure stockant les données de la partie.
typedef struct S_Partie {
	/// Numéro de la manche actuelle.
	int manche;
	/// Nombre de vies restantes.
	int vie;
	/// Carte posée sur le plateau. @c 0 si aucune.
	int cartePose;
	/// Identifiant du joueur ayant posé la carte.
	int idJCartePose;
	/// La dernière manche a-t-elle été gagnée ?
	bool lastRoundWon;
	/// Tableau contenant les joueurs (index du tableau = identifiant du joueur).
	Joueur joueurs[MAX_CONNECTIONS];
}Partie;

/**
 * @brief Définir les nouvelles cartes du joueur.
 * @param playercard Tableau des cartes.
 */
void distribuerCartes(const int* playercard);

/**
 * @brief Définir la carte posée sur le plateau.
 * @param carte Numéro de la carte (pas l'index).
 * @param idJCarte Identifiant du joueur qui a posé la carte.
 */
void setCartePose(int carte, int idJCarte);

/**
 * @brief Passer à la prochaine manche ou démarrer la première.
 * @param roundNumber Numéro de la manche.
 * @param lifeRemaining Nombre de vies restantes.
 * @param isLastRoundWon La dernière manche a-t-elle été gagnée ?
 */
void setNextRound(int roundNumber, int lifeRemaining, int isLastRoundWon);

/**
 * @brief Afficher l'écran de fin de partie.
 */
void finPartie();

/**
 * @brief Afficher l'écran de la manche en cours.
 */
void printManche();

/**
 * @brief Gérer la saisie de l'utilisateur dans une manche.
 * Permettre de lui saisir un numéro de carte pour la jouer.
 */
void gestionInputCarteJouer();

/**
 * @brief Gérer la saisie de l'utilisateur dans l'écran de fin de partie.
 * Permettre de lui saisir de recommencer une partie ou quitter le jeu.
 */
void gestionInputFinPartie();
