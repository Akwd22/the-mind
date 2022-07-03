/**
 * @file main.c
 * @brief Fichier de démarrage du programme.
 */

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "socket.h"
#include "logic/joueur.h"
#include "input.h"
#include "utils.h"

 /// Quitter le programme proprement à la réception des signaux SIGTERM et SIGINT.
void quit()
{
	exit(EXIT_SUCCESS);
}

/// Afficher le menu de connexion au serveur puis s'y connecter.
void connectMenu()
{
	char ip[15];
	unsigned short port;
	char name[20];

	do
	{
		clear();

		printf("-- Connexion à un serveur de jeu --\n");
		printf("Entrez les informations de connexion pour vous connecter à un serveur.\n\n");

		printf("Adresse IP du serveur : ");
		fgets(ip, sizeof(ip), stdin);

		printf("Port du serveur: ");
		scanf("%hu", &port);
		fgetc(stdin);

		printf("Tapez votre pseudo : ");
		fgets(name, sizeof(name), stdin);

		printf("\nConnexion au serveur...\n");

		ip[strcspn(ip, "\n")] = 0;
		name[strcspn(name, "\n")] = 0;

		setName(name);
	} while (!socket_connect(ip, port));
}

/// Initialiser l'état du programme.
void init()
{
	// Se déconnecter du serveur à la fermeture du programme.
	signal(SIGTERM, &quit);
	signal(SIGINT, &quit);
	atexit(&socket_disconnect);
}

int main(int argc, char* argv[])
{
	init();

	// Afficher le menu de connexion au serveur.
	connectMenu();

	// Démarrer la boucle de saisie utilisateur.
	beginInputLoop();

	return EXIT_SUCCESS;
}