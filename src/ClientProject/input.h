/**
 * @file input.h
 * @brief Interface permettant la gestion de la saisie de l'utilisateur dans le terminal.
 * 
 * Le programme est constamment en attente d'une saisie.
 * Après une saisie de l'utilisateur, une fonction de rappel est appelée pour effectuer une action.
 */

#pragma once

/**
 * @brief Définir la fonction de rappel lorsqu'une saisie a été faite par l'utilisateur dans l'entrée standard.
 * @param newPtrCallback Fonction de rappel (ou @c NULL si aucune fonction de rappel).
*/
void setInputCallback(void (*newPtrCallback)());

/**
 * @brief Retourner la dernière saisie de l'utilisateur dans l'entrée standard.
 * @return Chaîne de caractère saisie.
*/
char* getUserInput();

/**
 * @brief Démarrer la boucle capturant les saisies utilisateur dans l'entrée standard.
 * À chaque saisie, une fonction de rappel est appelée.
*/
void beginInputLoop();