/**
 * @file utils_io.h
 * @brief Interface pour la gestion de l'entrée/sortie d'un socket.
 */

#pragma once

#define SOCKET int

/**
 * @brief Lire des données à partir d'un flux d'octets.
 * Cette fonction assure la lecture de la totalité des données jusqu'au @p n i-éme octet.
 * @param fd Descripteur de fichier où récupérer les données du flux.
 * @param ptrBuffer Pointeur du buffer où stocker les données.
 * @param n Nombre d'octets à lire.
 * @return -1 si erreur, sinon 0.
*/
int recvn(int fd, void* ptrBuffer, size_t n);

/**
 * @brief Écrire des données dans flux d'octets.
 * Cette fonction assure l'écriture de la totalité des données jusqu'au @p n i-éme octet.
 * @param fd Descripteur de fichier où écrire les données.
 * @param ptrBuffer Pointeur du buffer où lire les données où écrire.
 * @param n Nombre d'octets à écrire.
 * @return -1 si erreur, sinon 0.
*/
int sendn(int fd, const void* ptrBuffer, size_t n);