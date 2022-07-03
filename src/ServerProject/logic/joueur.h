/**
 * @file joueur.h
 * @brief Interface permettant la gestion d'un joueur.
 */

#pragma once

#include <stdbool.h>

 /// Structure stockant les données d'un joueur.
typedef struct S_Joueur {
	/// Le joueur est-il prêt pour démarrer la partie ?
	bool ready;
	/// Nom du joueur.
	char nom[20];
	/// Identifiant du joueur.
	int id;
	/// Liste des cartes du joueur. La valeur de la carte vaut @c 0 si celle-ci a été jouée.
	int cartes[50];
}Joueur;

/**
 * @brief Définir le nom d'un joueur.
 * @param id Identifiant du joueur.
 * @param name Nouveau nom.
 */
void setPlayerName(int id, const char* name);

/**
 * @brief Définir un joueur prêt.
 * @param id Identifiant du joueur.
 */
void setReady(int id);