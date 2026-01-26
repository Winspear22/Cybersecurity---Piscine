/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spider.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:28:03 by adnen             #+#    #+#             */
/*   Updated: 2026/01/26 14:08:48 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "spider.hpp"

Spider::Spider()
{
	std::cout << BOLD_GREEN << "Spider constructor called" << RESET << std::endl;
}

Spider::Spider(const Spider &src)
{
	std::cout << BOLD_GREEN << "Spider copy constructor called" << RESET << std::endl;
	*this = src;
}

Spider &Spider::operator=(const Spider &src)
{
	std::cout << BOLD_GREEN << "Spider copy assignment operator called" << RESET << std::endl;
	if (this != &src) {}
	return *this;
}

Spider::~Spider()
{
	std::cout << BOLD_RED << "Spider destructor called" << RESET << std::endl;
}

void Spider::parse_arguments(const std::vector<std::string>& args)
{
	if (args.empty())
		return ;
	this->_start_url = args.back();
	std::cout << BOLD_GREEN << "Spider parse_arrguments called" << RESET << std::endl;
	std::cout << "URL cible définie : " << BOLD_YELLOW << _start_url << RESET << std::endl;
}

void Spider::run()
{
    // 1. On télécharge le code HTML du site
    std::cout << "Connexion à " << BOLD_YELLOW << _start_url << RESET << "..." << std::endl;
    std::string html_content = _request(_start_url);

    if (html_content.empty())
    {
        std::cerr << BOLD_RED << "Echec : impossible de télécharger la page !" << RESET << std::endl;
        return;
    }

    // 2. On analyse le HTML pour trouver les images (remplit _image_urls)
    _parse_html(html_content);

    // 3. On boucle sur toutes les images trouvées pour les télécharger
    std::cout << "Démarrage du téléchargement..." << std::endl;

    // --- Initialisation de l'itérateur pour la boucle while ---
    std::set<std::string>::iterator it = _image_urls.begin();

    while (it != _image_urls.end())
    {
        std::string image_url = *it;
        
        // Téléchargement de l'image
        std::string image_data = _request(image_url);
        
        // Sauvegarde sur le disque via le Saver
        if (!image_data.empty())
            _saver.save_file(image_data, image_url);
        // --- Incrémentation manuelle de l'itérateur ---
        ++it;
    }
    
    std::cout << BOLD_GREEN << "Terminé !" << RESET << std::endl;
}

// ------------------------------------------------------------------
// PARTIE 1 : LA CONFIGURATION ET L'ENVOI (Le "Patron")
// ------------------------------------------------------------------

std::string Spider::_request(const std::string& url)
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
// ------------------------------------------------------------------
// PARTIE 2 : LE CALLBACK (Le "Magasinier")
// ------------------------------------------------------------------
// Note : Pas de mot "static" ici ! Il est seulement dans le .hpp
// Cette fonction est appelée automatiquement par libcurl quand des paquets arrivent.

size_t Spider::_write_callback(void *contents, size_t size, size_t nmemb, void *userp)
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

void Spider::_parse_html(const std::string& html)
{
	std::regex regex(R"(<img[^>]*src="([^"]*))");
	std::sregex_iterator it(html.begin(), html.end(), regex);
	std::sregex_iterator end;
	while (it != end)
	{
		std::smatch match = *it;
		if (match.size() > 1)
		{
			std::string url = match[1];
			if (url.find("http") != std::string::npos)
				_image_urls.insert(url);
			else
				_image_urls.insert(_start_url + url);
		}
		++it;
	}
	std::cout << BOLD_CYAN << "🔍 Analyse terminée : " << _image_urls.size() << " images trouvées." << RESET << std::endl;
}
