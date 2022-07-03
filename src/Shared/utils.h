/**
 * @file utils.h
 * @brief Bibliothèque de fonctions, macros, et constantes utilitaires.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

/// Afficher les informations de débogage ?
#define DEBUG_MODE 0

// Couleur d'écriture dans le terminal.
#define TERM_DEFAULT  "\033[0m"
#define TERM_BLACK    "\033[0;30m"
#define TERM_GREY     "\033[1;30m"
#define TERM_RED      "\033[0;31m"
#define TERM_GREEN    "\033[0;32m"
#define TERM_YELLOW   "\033[0;33m"
#define TERM_BLUE     "\033[1;34m"
#define TERM_PURPLE   "\033[0;35m"
#define TERM_CYAN     "\033[0;36m"
#define TERM_WHITE    "\033[0;37m"

/**
 * @def printfc(color, msg, ...)
 * @brief Imprimer un message formaté dans une couleur spécifique dans la sortie standard.
 * @param color Séquence d'échappement ANSI. Voir les constantes @c TERM_<COULEUR> pour les couleurs prédéfinies.
 */
#define printfc(color, format, ...) { printf(color format TERM_DEFAULT, ##__VA_ARGS__); }

/**
 * @def pDebug(msg, ...)
 * @brief Imprimer un message formaté jaune dans la sortie standard uniquement en mode débogage.
 * La constante @link DEBUG_MODE @endlink doit être à @c 1.
 */
#define pDebug(format, ...) { if (DEBUG_MODE) printf(TERM_YELLOW format TERM_DEFAULT, ##__VA_ARGS__); }

/**
 * @def FATAL_ERR(msg)
 * @brief Afficher l'erreur de la dernière fonction appelée avec un @p msg dans la sortie d'erreur et quitter le programme.
 */
#define FATAL_ERR(msg) { perror(msg); kill(getpid(), SIGTERM); }

/**
 * @def clear()
 * @brief Effacer le terminal uniquement en mode non-débogage.
 * La constante @link DEBUG_MODE @endlink doit être à @c 0.
 */
#define clear() { if (!DEBUG_MODE) system("clear"); }

/**
 * @def TO_STR(x)
 * @def TO_STR_INDIR(x)
 * @brief Convertir un entier littéral en string.
 */
#define TO_STR_INDIR(x) #x
#define TO_STR(x) TO_STR_INDIR(x)

/// Remplacer tous les caractères @p toFind par @p toReplace dans un @p string de taille @p size caractères.
void strReplaceChar(char* string, char toFind, char toReplace, size_t size);