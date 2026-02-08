/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Downloader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:45:21 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 14:30:32 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Downloader.hpp"

Downloader::Downloader() {}
Downloader::~Downloader() {}
Downloader::Downloader(const Downloader &src) { *this = src; }
Downloader &Downloader::operator=(const Downloader &src) 
{ 
	if (this != &src) {} 
	return *this; 
}


std::string Downloader::curl(const std::string& url)
{
    CURL        *curl = curl_easy_init();
    std::string readBuffer;
    CURLcode    res;

    // Sécurité : Si l'init échoue, on arrête tout de suite.
    if (!curl) {
        std::cerr << BOLD_RED << "Erreur critique : Impossible d'initialiser CURL." << RESET << std::endl;
        return "";
    }

    // ---------------------------------------------------------
    // 1. CONFIGURATION DU ROBOT
    // ---------------------------------------------------------
    
    // L'URL cible
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Le Callback (Le magasinier)
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_callback);
    
    // Le Buffer (Le sac où ranger les données)
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    
    // Suivre les redirections (http -> https ou www.)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	// User Agent (on se fait passer pour mozzilla firefox)
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");

    // [ZONE DANGEREUSE] : Options SSL
    // Décommenter pour accepter les certificats auto-signés (ex: self-signed.badssl.com)
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // ---------------------------------------------------------
    // 2. EXÉCUTION
    // ---------------------------------------------------------
    
    res = curl_easy_perform(curl);

    // ---------------------------------------------------------
    // 3. GESTION DES ERREURS
    // ---------------------------------------------------------
    
    if (res != CURLE_OK)
    {
        // En cas d'erreur, on vide le buffer pour ne pas renvoyer de données corrompues
        readBuffer.clear();

        std::cerr << BOLD_RED << "❌ Erreur Curl (" << res << ") : " 
                  << curl_easy_strerror(res) << RESET << std::endl;

        // Message spécifique pour le certificat SSL (Code 60)
        if (res == CURLE_PEER_FAILED_VERIFICATION)
        {
            std::cerr << BOLD_YELLOW << "Erreur : Certificat SSL invalide." << std::endl;
            std::cerr << "    Le site " << url << " possède un certificat auto-signé ou expiré." << std::endl;
            std::cerr << "    Connexion refusée par défaut." << RESET << std::endl;
        }
    }

    // ---------------------------------------------------------
    // 4. NETTOYAGE
    // ---------------------------------------------------------
    
    curl_easy_cleanup(curl);
    
    return readBuffer;
}

size_t Downloader::_write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    // A. Calcul de la taille réelle du paquet reçu
    // Curl donne la taille en "nombre de blocs" * "taille du bloc".
    size_t realsize = size * nmemb;

    // B. Récupération du sac (Casting)
    // "userp" est un pointeur générique (void*). C'est l'adresse de &readBuffer qu'on a passée plus haut.
    // On doit dire au compilateur : "Je te jure que c'est un pointeur vers une std::string".
    std::string *buffer = static_cast<std::string*>(userp);

    // C. Remplissage
    // On ajoute (append) les données brutes (castées en char*) à la fin de notre string.
    buffer->append((char*)contents, realsize);

    // D. Validation
    // On renvoie la taille traitée. Si on renvoie un chiffre différent de realsize,
    // libcurl pensera qu'il y a eu une erreur d'écriture et coupera la connexion.
    return realsize;
}