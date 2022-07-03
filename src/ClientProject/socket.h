/**
 * @file socket.h
 * @brief Interface permettant de gérer la connexion socket au serveur.
 */

#pragma once

#include <stdbool.h>
#include "messaging/structs.h"
#include "messaging/enums.h"

/**
 * @brief Se connecter à un serveur distant.
 * @param ip Adresse IP du serveur.
 * @param port Port du serveur.
 * @return @c true si connexion réussie, sinon @c false.
*/
bool socket_connect(const char* ip, unsigned short port);

/**
 * @brief Se déconnecter du serveur actuel.
*/
void socket_disconnect();

/**
 * @brief Envoyer un message au serveur.
 * @param type Type du message.
 * @param msg Données utiles du message. Pointeur vers une structure de @link messaging/structs.h @endlink. @c NULL si aucune donnée utile.
 * @param size Taille du message. En général, taille de la structure passée en @p msg. @c 0 si aucune donnée utile.
*/
void socket_send(enum CliMsg type, const void* msg, size_t size);