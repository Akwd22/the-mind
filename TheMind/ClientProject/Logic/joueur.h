/**
 * @file joueur.h
 * @brief Interface permettant la gestion du joueur.
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>

/// Structure stockant les données du joueur.
typedef struct S_Joueur { // TODO : vérifi attributs utilisés
	/// Le joueur est-il prêt pour démarrer la partie ?
	bool ready;
	/// Nom du joueur.
	char nom[20];
	/// Identifiant du joueur.
	int id;
	/// Nombre de cartes du joueur.
	int nbCartes;
	/// Liste des cartes du joueur. La valeur de la carte vaut @a 0 si celle-ci a été jouée.
	int cartes[50];
}joueur;

/// Instance du joueur.
extern joueur j; // TODO : rename

/**
 * @brief Définir le nom du joueur.
 * @param nom Nom du joueur.
 */
void setName(char * nom);

/**
 * @brief Jouer une carte et l'envoyer au serveur.
 * @param carte Index de la carte.
 */
void jouerCarte(int carte);

/**
 * @brief Définir l'identifiant du joueur.
 * @param id Identifiant du joueur.
 */
void setId(int id);