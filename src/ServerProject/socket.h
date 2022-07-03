/**
 * @file socket.h
 * @brief Interface permettant de gérer la connexion socket avec les clients.
 */

#pragma once

#include <stddef.h>
#include "messaging/structs.h"
#include "messaging/enums.h"

 /// Port du serveur.
#define SERVER_PORT 25565

/**
 * @brief Démarrer le serveur.
 * Démarrer le socket, écouter les demandes de connexion, et gérer la communication avec les clients.
*/
void socket_open();

/**
 * @brief Fermer le serveur.
 * Terminer la connexion des clients et du serveur.
 * @warning Ne pas appeler cette fonction autre que dans le thread principal.
 * Si vous voulez fermer le serveur depuis un autre thread, utilisez @link socket_requestClose() @endlink à la place.
*/
void socket_close();

/**
 * @brief Faire une requête pour fermer le serveur.
 * @note Cette fonction est utilisée pour fermer le serveur depuis un thread.
 * Elle appelle @link socket_close() @endlink dans le thread principal.
*/
void socket_requestClose();

/**
 * @brief Diffuser un message à tous les clients connectés au serveur.
 * @param type Type du message.
 * @param msg Données utiles du message. Pointeur vers une structure de @link messaging/structs.h @endlink. @c NULL si aucune donnée utile.
 * @param size Taille du message. En général, taille de la structure passée en @p msg. @c 0 si aucune donnée utile.
*/
void socket_broadcast(enum SrvMsg type, const void* msg, size_t size);

/**
 * @brief Envoyer un message à un client.
 * @param clientId Identifiant du client.
 * @param type Type du message.
 * @param msg Données utiles du message. Pointeur vers une structure de @link messaging/structs.h @endlink. @c NULL si aucune donnée utile.
 * @param size Taille du message. En général, taille de la structure passée en @p msg. @c 0 si aucune donnée utile.
*/
void socket_send(unsigned int clientId, enum SrvMsg type, const void* msg, size_t size);

/**
 * @brief Démarrer et connecter des robots au serveur.
 * @param nb Nombre de robots à connecter.
*/
void socket_bots(unsigned int nb);