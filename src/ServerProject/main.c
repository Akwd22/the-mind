/**
 * @file main.c
 * @brief Fichier de démarrage du programme.
 */

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <libgen.h>
#include <time.h>
#include <linux/limits.h>
#include "socket.h"

 /// Quitter le programme proprement à la réception des signaux SIGTERM et SIGINT.
void quit()
{
	exit(EXIT_SUCCESS);
}

/// Initialiser l'état du programme.
void init()
{
	srand(time(NULL));

	// Déconnecter les clients et le serveur à la fermeture du programme.
	signal(SIGTERM, &quit);
	signal(SIGINT, &quit);
	atexit(&socket_close);

	// Changer le répertoire courant vers le répertoire où se trouve l'exécutable.
	char path[PATH_MAX] = "";
	readlink("/proc/self/exe", path, sizeof(path));
	chdir(dirname(path));
}

int main(int argc, char* argv[])
{
	init();
	socket_open();

	return EXIT_SUCCESS;
}
