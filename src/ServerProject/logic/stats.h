/**
 * @file stats.h
 * @brief Interface permettant la gestion des statistiques du jeu et des joueurs.
 */

#pragma once

#include <stdbool.h>

 /// Structure stockant les statistiques du jeu et de l'ensemble des parties jouées.
typedef struct S_GlobalStats {
	/// Nombre de parties jouées.
	unsigned int gameCount;
	/// Temps de réaction minimum parmi tous les joueurs (en secondes).
	unsigned int minReactionTime;
	/// Temps de réaction maximum parmi tous les joueurs (en secondes).
	unsigned int maxReactionTime;
	/// Temps de réaction moyen parmi tous les joueurs (en secondes).
	float avgReactionTime;
	/// Nombre moyen de manches gagnées pour une partie.
	float avgRoundWon;
	/// Identifiant du joueur qui a fait perdre le plus de fois une manche.
	unsigned int worstPlayerId;
	/// Tableau stockant l'historique du nombre de manches gagnées de chaque partie jouée.
	unsigned int roundWonPerGame[50];
} GlobalStats;

/// Structure stockant les statistiques d'un joueur.
typedef struct S_PlayerStats {
	/// Temps de réaction minimum (en secondes).
	unsigned int minReactionTime;
	/// Temps de réaction maximum (en secondes).
	unsigned int maxReactionTime;
	/// Temps de réaction moyen (en secondes).
	float avgReactionTime;
	/// Nombre de fois dont il a causé la perte d'une manche.
	unsigned int failCount;
} PlayerStats;

/**
 * @brief Générer le PDF des statistiques du jeu et des joueurs.
 *
 * Le fichier généré se trouve dans le répertoire @c game_stats à la racine du programme.
 */
void stats_generatePDF();

/**
 * @brief Retourner le nombre de secondes écoulées depuis le dernier appel de cette fonction.
 *
 * @param resetTimer Faut-il réinitialiser le compteur ?
 * Si @c true, le prochain appel donnera le temps écoulé depuis cet appel avec @p resetTimer.
 *
 * @return unsigned int Nombre de secondes depuis le dernier appel. @c 0 si c'est le premier appel, ou si @p resetTimer vaut @c true.
 */
unsigned int stats_elapsedSecs(bool resetTimer);

/* -------------------------- Statistiques globales ------------------------- */

/**
 * @brief Mettre à jour les temps de réaction globaux.
 * @param reactionTime Temps de réaction mesuré (en secondes).
 */
void stats_updateReactionTimes(unsigned int reactionTime);

/**
 * @brief Mettre à jour les statistiques globales du jeu.
 */
void stats_updateGameStats();

/* ------------------------ Statistiques des joueurs ------------------------ */

/**
 * @brief Mettre à jour les temps de réaction d'un joueur.
 *
 * @param id Identifiant du joueur concerné.
 * @param reactionTime Temps de réaction mesuré (en secondes).
 */
void stats_updatePlayerReactionTimes(unsigned int id, unsigned int reactionTime);

/**
 * @brief Incrémenter le nombre d'échecs d'un joueur.
 * @param id Identifiant du joueur concerné.
 */
void stats_updatePlayerFailCount(unsigned int id);