/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spider.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:28:03 by adnen             #+#    #+#             */
/*   Updated: 2026/01/26 12:01:48 by adnen            ###   ########.fr       */
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
	std::cout << BOLD_GREEN << "Spider run called" << RESET << std::endl;
	// 1. Vérification de sécurité
    if (_start_url.empty())
    {
        std::cerr << BOLD_RED << "Erreur : Aucune URL n'a été fournie !" << RESET << std::endl;
        return;
    }
	// 2. Appel de ta fonction _request (celle avec libcurl)
    std::cout << "Connexion à " << _start_url << "..." << std::endl;
    std::string html_content = _request(_start_url);
	if (html_content.empty())
		std::cout << BOLD_RED << "Echec : impossible de télécharger la page !" << std::endl;
	else
	{
		std::cout << BOLD_GREEN << "Succès ! " << html_content.size() << " octets reçus." << RESET << std::endl;
		std::cout << CYAN << html_content.substr(0, 500) << RESET << "..." << std::endl;	}
	
}

// ------------------------------------------------------------------
// PARTIE 1 : LA CONFIGURATION ET L'ENVOI (Le "Patron")
// ------------------------------------------------------------------

std::string Spider::_request(const std::string& url)
{
    CURL        *curl;          // Le "handle" : l'objet qui représente ta session
    CURLcode    res;            // Le code de retour pour savoir si ça a marché
    std::string readBuffer;     // LE SAC : La string vide qui va se remplir

	(void)res;
    // 1. Initialisation : On demande à la libcurl de préparer une session
    curl = curl_easy_init();
    
    // On vérifie toujours que l'init a fonctionné (sinon risque de crash)
    if (curl)
    {
        // 2. Configuration : On colle les étiquettes sur le colis

        // OPTION URL : On dit où on veut aller.
        // .c_str() est OBLIGATOIRE car libcurl est en C, elle ne comprend pas std::string direct.
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // OPTION FONCTION : On désigne le "magasinier".
        // On donne l'adresse de notre fonction statique _write_callback.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_callback);

        // OPTION DONNÉES : On désigne le "sac".
        // On passe l'ADRESSE (&) de readBuffer pour que le callback puisse écrire DEDANS.
        // C'est ici que le lien se fait entre ta variable locale et la librairie C.
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // 3. Exécution : C'est le grand saut !
        // Le programme se met en pause ici le temps que le téléchargement se fasse.
        // Pendant ce temps, curl va appeler _write_callback plusieurs fois.
        res = curl_easy_perform(curl);

        // (Optionnel mais conseillé : vérifier si res != CURLE_OK ici pour gérer les erreurs)

        // 4. Nettoyage : On rend la mémoire prêtée par la librairie
        curl_easy_cleanup(curl);
    }
    
    // On retourne le sac désormais rempli de HTML !
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