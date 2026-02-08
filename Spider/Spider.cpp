/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spider.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:28:03 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 14:54:33 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Spider.hpp"

Spider::Spider()
{
	this->_recursive = false;
	this->_max_depth = 5;
	this->_output_dir_path = "./data/";
	this->_invalid_images_count = 0;
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
	if (this != &src)
	{
		this->_start_url = src._start_url;
		this->_image_urls = src._image_urls;
		this->_args = src._args;
		this->_url_queue = src._url_queue;
		this->_visited_urls = src._visited_urls;
		this->_recursive = src._recursive;
		this->_max_depth = src._max_depth;
		this->_output_dir_path = src._output_dir_path;
		this->_invalid_images_count = src._invalid_images_count;
		
		// Les objets helpers (stateless pour l'instant)
		this->_urlHelper = src._urlHelper;
		this->_downloader = src._downloader;
		this->_parser = src._parser;
		this->_saver = src._saver;
	}
	return *this;
}

Spider::~Spider()
{
	std::cout << BOLD_RED << "Spider destructor called" << RESET << std::endl;
}

void Spider::addArgumentsToVector(char **argv)
{
	int i;

	i = 1;
	while (argv[i] != NULL)
	{
		this->_args.push_back(argv[i]);
		i++;
	}
	std::cout << BOLD_GREEN << "Spider parse_arrguments called" << RESET << std::endl;
	std::cout << "URL cible définie : " << BOLD_YELLOW << _start_url << RESET << std::endl;
}

bool Spider::argsParser(void)
{
	std::vector<std::string>::iterator it;

	it = this->_args.begin();
	while (it != this->_args.end())
	{
		if (*it == "-r")
			this->_recursive = true;
		else if (*it == "-l")
		{
			if (it + 1 == this->_args.end())
				return print_error("Error : -l option requires an argument");
			if (it + 1 != this->_args.end())
			{
				it++;
				try
				{
					this->_max_depth = std::stoi(*it);
					if (this->_max_depth < 0)
						return print_error("Error : max depth must be a positive integer");
				} 
				catch (const std::exception& e)
				{
					    std::string msg = "Error : invalid depth value (" + std::string(e.what()) + ")";
						return print_error(msg);
				}
			}
		}
		else if (*it == "-p")
		{
			if (it + 1 == this->_args.end())
				return print_error("Error : -p option requires an argument");
			if (it + 1 != this->_args.end())
			{
				it++;
				this->_output_dir_path = *it;
			}
		}
		else
		{
			if (it + 1 != this->_args.end())
				return print_error("Error : URL must be the last argument (flags come first)");
			this->_start_url = *it;
		}
		it++;
	}
	if (this->_start_url.empty())
		return print_error("Error : No URL provided. Usage: ./spider [-rlp] URL");
	return print_success("Arguments parsed successfully");
}

void Spider::run()
{
    std::cout << BOLD_GREEN << "🕷️ Démarrage du Spider sur : " << this->_start_url << RESET << std::endl;

    // 1. Initialisation : On met l'URL de départ dans la file d'attente
    this->_url_queue.push_back(std::make_pair(_start_url, 0));
    this->_visited_urls.insert(_start_url);

    // 2. BOUCLE PRINCIPALE (BFS) : Tant qu'il y a des liens à visiter...
    while (!this->_url_queue.empty())
    {
        // A. On récupère le prochain lien à traiter
        std::string current_url = this->_url_queue.front().first;
        int current_depth = this->_url_queue.front().second;
        this->_url_queue.erase(this->_url_queue.begin()); // On l'enlève de la liste

        // B. PAUSE POLITESSE (50ms) pour éviter le ban
        usleep(50000); 

        // C. Téléchargement de la page HTML
        std::cout << "⏳ Traitement (Prof " << current_depth << ") : " << current_url << " ... ";
        std::string html_content = _downloader.curl(current_url);

        if (html_content.empty())
        {
            std::cout << BOLD_RED << "❌ (Vide/Erreur)" << RESET << std::endl;
            continue; // On passe au suivant
        }
        std::cout << BOLD_GREEN << "✅ OK" << RESET << std::endl;

        // D. Analyse des images (Remplissage de _image_urls)
        std::set<std::string> new_images = _parser.extract_images(html_content, _start_url, _urlHelper);
        _image_urls.insert(new_images.begin(), new_images.end());

        // E. Analyse des liens (Récursivité) -> Ajoute les nouveaux liens dans la queue
        if (_recursive && current_depth < _max_depth)
        {
             std::vector<std::string> new_links = _parser.extract_links(html_content, _start_url, _urlHelper);
             
             // On ajoute les nouveaux liens à la queue BFS
             for (size_t i = 0; i < new_links.size(); i++)
             {
                 std::string link = new_links[i];
                 if (_visited_urls.find(link) == _visited_urls.end())
                 {
                     if (_is_safe_to_scan(link) == SUCCESS) // On réutilise is_safe pour vérifier domaine/doublons
                     {
                         _visited_urls.insert(link);
                         _url_queue.push_back(std::make_pair(link, current_depth + 1));
                         std::cout << "  [BFS] Ajout lien (Prof " << current_depth + 1 << ") : " << link << std::endl;
                     }
                 }
             }
        }
    }

    // 3. TÉLÉCHARGEMENT MASSIF DES IMAGES (Une fois l'exploration finie)
    std::cout << BOLD_CYAN << "\n📦 Exploration terminée ! " << _image_urls.size() << " images uniques trouvées." << RESET << std::endl;
    std::cout << "⬇️  Démarrage du téléchargement..." << std::endl;

    std::set<std::string>::iterator it = _image_urls.begin();
    while (it != _image_urls.end())
    {
        std::string image_url = *it;
        
        // Téléchargement (avec petite pause aussi)
        std::cout << "   -> " << image_url << std::endl;
        std::string image_data = _downloader.curl(image_url);
        usleep(10000); // 10ms entre chaque image
        
        if (!image_data.empty())
            _saver.save_file(image_data, image_url, _output_dir_path);
        
        ++it;
    }

    // 4. Résumé final
    if (_image_urls.empty() && _invalid_images_count > 0)
		std::cerr << BOLD_RED << "Error, TOUTES les images (" << _invalid_images_count << ") étaient invalides." << RESET << std::endl;
	else
	{
		std::cout << BOLD_GREEN << "Terminé !" << RESET << std::endl;
		if (_invalid_images_count > 0)
			std::cout << BOLD_YELLOW << "Succès ! Mais il y'avait " << _invalid_images_count << " images rejetées." << RESET << std::endl;
	}
}

// ------------------------------------------------------------------
// PARTIE 1 : LA CONFIGURATION ET L'ENVOI (Le "Patron")
// ------------------------------------------------------------------


// ------------------------------------------------------------------
// PARTIE 2 : LE CALLBACK (Le "Magasinier")
// ------------------------------------------------------------------
// Note : Pas de mot "static" ici ! Il est seulement dans le .hpp
// Cette fonction est appelée automatiquement par libcurl quand des paquets arrivent.




bool Spider::_is_safe_to_scan(std::string& url)
{
	size_t i;
	
	// 1. On nettoie les ancres (#section)
	size_t hash_pos = url.find('#');
	if (hash_pos != std::string::npos)
		url = url.substr(0, hash_pos);

	// 2. Vérification du protocole (HTTP/HTTPS uniquement)
	if (url.find("http") != 0 && url.find("https") != 0)
		return (print_error("[IGNORÉ] Protocole invalide : " + url));

	// 3. Vérification des extensions interdites
	std::vector<std::string> banned_exts = {".pdf", ".zip", ".rar", ".tar", ".xml", ".json", ".iso", ".css", ".js"};
	
	i = 0;
	while (i < banned_exts.size())
	{
		if (url.length() >= banned_exts[i].length() && 
			url.compare(url.length() - banned_exts[i].length(), banned_exts[i].length(), banned_exts[i]) == 0)
		{
			return (print_error("[IGNORÉ] Extension interdite (" + banned_exts[i] + ") : " + url));
		}
		i++;
	}

	// 4. Vérification de l'historique (Pas de message ici, sinon trop de spam)
	if (_visited_urls.find(url) != _visited_urls.end())
		return (FAILURE);

	// 5. Vérification du Domaine
	std::string start_domain = _start_url;
	size_t protocol_end = start_domain.find("://");
	if (protocol_end != std::string::npos)
		start_domain = start_domain.substr(protocol_end + 3);
	size_t path_start = start_domain.find("/");
	if (path_start != std::string::npos)
		start_domain = start_domain.substr(0, path_start);

	if (url.find(start_domain) == std::string::npos)
		return (print_error("[IGNORÉ] Hors domaine : " + url));

	return (SUCCESS);
}