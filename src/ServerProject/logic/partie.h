/**
 * @file partie.h
 * @brief Interface permettant de gérer une partie et les manches.
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "socket/consts.h"
#include "joueur.h"

 /// Structure stockant les données de la partie.
typedef struct S_Partie {
	/// Numéro de la manche actuelle.
	int manche;
	/// Nombre de joueurs.
	int nbJoueurs;
	/// Nombre de vies restantes.
	int vie;
	/// Carte posée sur le plateau. @c 0 si aucune.
	int terrainJeu;
	/// Tableau contenant les joueurs (index du tableau = identifiant du joueur).
	Joueur joueurs[MAX_CONNECTIONS];
}Partie;

/// Instance de la partie.
extern Partie partie;

/**
 * @brief Initialiser la partie et démarrer la première manche.
 * @param tab Tableau des joueurs provenant du lobby.
 * @param n Nombre de joueurs.
 */
void initPartie(Joueur tab[], int n);

/**
 * @brief Gérer une carte qu'un joueur vient de jouer.
 *
 * Effectuer les vérifications nécessaires, à savoir :
 * 	- La carte fait-elle gagnée la manche ? Si oui, passer à la manche suivante.
 * 	- La carte fait-elle perdre la manche ? Si oui, faire perdre une vie, ou game over.
 * 	- La carte fait-elle continuer la manche ? Si oui, poser la carte sur le plateau.
 *
 * @param idJoueur Identifiant du joueur jouant la carte.
 * @param idCarte Index de la carte.
 * @return @c true si la manche continue.
 */
bool gestionCarteJouer(int idJoueur, int idCarte);