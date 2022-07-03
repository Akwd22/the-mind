/**
 * @file stats.c
 * @brief Implémentation de la gestion des statistiques du jeu et des joueurs.
 */

#include <time.h>
#include <stdbool.h>
#include <sds/sds.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../socket.h"
#include "partie.h"
#include "utils.h"
#include "stats.h"

 /// Instance des statistiques globales.
GlobalStats globalStats = { 0 };
/// Tableau des instances des statistiques de chaque joueur.
PlayerStats playerStats[MAX_CONNECTIONS] = { 0 };

void stats_generatePDF()
{
	// Remplacer les espaces par des - sinon la commande ne s'exécute pas.
	for (unsigned int i = 0; i < (unsigned int)partie.nbJoueurs; i++)
	{
		strReplaceChar(partie.joueurs[i].nom, ' ', '-', sizeof(partie.joueurs[i].nom));
	}

	// ---- Création des paramètres d'exécution de Awk. ----

	sds args = sdsnew("");

	// Statistiques globales.
	args = sdscatprintf(args,
		" -v PLAYER_COUNT=%i"\
		" -v REACTION_MIN=%i"\
		" -v REACTION_MAX=%i"\
		" -v REACTION_AVG=%.2f"\
		" -v ROUND_WON_AVG=%.2f"\
		" -v WORST_PLAYER=%s",
		partie.nbJoueurs,
		globalStats.minReactionTime,
		globalStats.maxReactionTime,
		globalStats.avgReactionTime,
		globalStats.avgRoundWon,
		partie.joueurs[globalStats.worstPlayerId].nom
	);

	// Nombre de manches gagnées pour chaque partie.
	args = sdscat(args, " -v ROUND_WON_PER_GAME=");
	for (unsigned int i = 0; i < globalStats.gameCount; i++)
	{
		args = sdscatprintf(args, "(%i,%i)", i, globalStats.roundWonPerGame[i]);
	}

	// Statistiques de chaque joueur.
	args = sdscat(args, " -v PLAYERS_ARRAY=");
	for (unsigned int i = 0; i < (unsigned int)partie.nbJoueurs; i++)
	{
		args = sdscatprintf(args, "%s:%i:%i:%.2f:%i|",
			partie.joueurs[i].nom,
			playerStats[i].minReactionTime,
			playerStats[i].maxReactionTime,
			playerStats[i].avgReactionTime,
			playerStats[i].failCount
		);
	}

	// ---- Exécuter le script shell générant le fichier PDF et lui passer les paramètres. ----
	int scriptPid;

	if ((scriptPid = fork()) == 0)
	{
		if (execlp("./template/generate_pdf.sh", "./template/generate_pdf.sh", args, NULL) == -1)
		{
			perror("stats_generatePDF - execlp()");
		}

		sdsfree(args);
		return;
	}

	if (scriptPid == -1)
	{
		perror("stats_generatePDF - fork()");
	}

	// Attendre la fin du script shell.
	if (waitpid(scriptPid, NULL, 0) == -1)
	{
		perror("stats_generatePDF - waitpid()");
	}

	sdsfree(args);
}

unsigned int stats_elapsedSecs(bool resetTimer)
{
	static bool isFirst = true;
	static time_t start;

	isFirst = resetTimer;

	if (isFirst)
	{
		isFirst = false;
		time(&start);
		return 0;
	}

	isFirst = true;

	time_t end;
	time(&end);

	return (end - start);
}

/* -------------------------- Statistiques globales ------------------------- */

/// Mettre à jour le nombre moyen de manches gagnées par partie.
void stats_updateAvgRoundWon()
{
	static unsigned int currentSum = 0;
	currentSum += partie.manche - 1;

	globalStats.avgRoundWon = (float)currentSum / globalStats.gameCount;
}

/// Mettre à jour le pire joueur.
void stats_updateWorstPlayer()
{
	unsigned int worstPlayerId = 0;
	unsigned int highestFailCount = 0;

	for (int i = 0; i < partie.nbJoueurs; i++)
	{
		unsigned int lostCount = playerStats[i].failCount;

		if (lostCount > highestFailCount)
		{
			worstPlayerId = i;
			highestFailCount = lostCount;
		}
	}

	globalStats.worstPlayerId = worstPlayerId;
}

void stats_updateReactionTimes(unsigned int reactionTime)
{
	if (globalStats.minReactionTime == 0) globalStats.minReactionTime = reactionTime;
	if (reactionTime < globalStats.minReactionTime) globalStats.minReactionTime = reactionTime;
	if (reactionTime > globalStats.maxReactionTime) globalStats.maxReactionTime = reactionTime;

	// Calcul du temps de réaction moyen.
	static unsigned int currentN = 0;
	static float currentSum = 0;

	currentN++;
	currentSum += reactionTime;

	globalStats.avgReactionTime = currentSum / currentN;
}

void stats_updateGameStats()
{
	globalStats.roundWonPerGame[globalStats.gameCount++] = partie.manche - 1;
	stats_updateAvgRoundWon();
	stats_updateWorstPlayer();
}

/* ------------------------ Statistiques des joueurs ------------------------ */

void stats_updatePlayerReactionTimes(unsigned int id, unsigned int reactionTime)
{
	PlayerStats* stats = &playerStats[id];

	if (stats->minReactionTime == 0) stats->minReactionTime = reactionTime;
	if (reactionTime < stats->minReactionTime) stats->minReactionTime = reactionTime;
	if (reactionTime > stats->maxReactionTime) stats->maxReactionTime = reactionTime;

	// Calcul du temps de réaction moyen.
	static unsigned int currentN[MAX_CONNECTIONS];
	static float currentSum[MAX_CONNECTIONS];

	currentN[id]++;
	currentSum[id] += reactionTime;

	stats->avgReactionTime = currentSum[id] / currentN[id];
}

void stats_updatePlayerFailCount(unsigned int id)
{
	playerStats[id].failCount++;
}