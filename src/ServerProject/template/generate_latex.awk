substr($1, 0, 1) != "%" { print($0) }

$1 == "%PLAYER_COUNT" { $1 = ""; printf($0, PLAYER_COUNT) }

$1 == "%REACTION_MIN" { $1 = ""; printf($0, REACTION_MIN) }
$1 == "%REACTION_MAX" { $1 = ""; printf($0, REACTION_MAX) }
$1 == "%REACTION_AVG" { $1 = ""; printf($0, REACTION_AVG) }

$1 == "%ROUND_WON_AVG" { $1 = ""; printf($0, ROUND_WON_AVG) }
$1 == "%WORST_PLAYER" { $1 = ""; printf($0, WORST_PLAYER) }

$1 == "%ROUND_WON_PER_GAME" { $1 = ""; printf($0, ROUND_WON_PER_GAME) }

$1 == "%PLAYER_STATS" {
	$1 = ""

	split(PLAYERS_ARRAY, playerStatsArray, "|")

	for (i = 1; i <= PLAYER_COUNT; i++)
	{
		split(playerStatsArray[i], playerStats, ":")

		playerName = playerStats[1]
		minReactionTime = playerStats[2]
		maxReactionTime = playerStats[3]
		avgReactionTime = playerStats[4]
		failCount = playerStats[5]

		printf("\\hline %s & %d secondes & %d secondes & %.2f secondes & %d \\\\", playerName, minReactionTime, maxReactionTime, avgReactionTime, failCount)
	}
}