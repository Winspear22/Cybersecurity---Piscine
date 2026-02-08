/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Saver.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:56:19 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 01:28:30 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Saver.hpp"
Saver::Saver()
{
	std::cout << BOLD_BLUE << "Saver constructor called" << RESET << std::endl;
}

Saver::Saver(const Saver &src)
{
	std::cout << BOLD_GREEN << "Saver copy constructor called" << RESET << std::endl;
	*this = src;
}

Saver &Saver::operator=(const Saver &src)
{
	std::cout << BOLD_GREEN << "Saver copy assignment operator called" << RESET << std::endl;
	if (this != &src) {}
	return *this;
}

Saver::~Saver()
{
	std::cout << BOLD_BLUE << "Saver destructor called" << RESET << std::endl;
}

// void Saver::save_file(const std::string& data, const std::string& url)
void Saver::save_file(const std::string& data, const std::string& url, const std::string& output_dir)
{
	// 1. Création du dossier (si nécessaire)
	mkdir(output_dir.c_str(), 0777);

	std::string cleanName = this->_extract_filename(url); // ON RECUPERE LE NOM ICI
	
	// 3. Construction du chemin complet
	std::string fullPath;
	if (output_dir[output_dir.length() - 1] == '/')
		fullPath = output_dir + cleanName;
	else
		fullPath = output_dir + "/" + cleanName;

	// --- Gestion des doublons (image.jpg -> image_1.jpg) ---
	std::string baseName = fullPath;
	std::string extension = "";
	size_t extPos = fullPath.find_last_of('.');
	
	if (extPos != std::string::npos)
	{
		baseName = fullPath.substr(0, extPos);
		extension = fullPath.substr(extPos);
	}

	int counter = 1;
	struct stat buffer;
	while (stat(fullPath.c_str(), &buffer) == 0) // Tant que le fichier existe
    {
		// VERSION SIMPLE (sans ostringstream)
		fullPath = baseName + "_" + std::to_string(counter) + extension;
		counter++;
	}
	// ---------------------------------------------------------------

	// 4. Écriture
	std::ofstream file(fullPath.c_str(), std::ios::out | std::ios::binary);	
	if (!file.is_open())
	{
		std::cerr << BOLD_RED << "Error: could not write to " << fullPath << RESET << std::endl;
		return ;
	}
	file.write(data.c_str(), data.size());
	file.close();
}

std::string Saver::_extract_filename(const std::string& url)
{
	std::string filename;

	// 1. On garde tout ce qui est après le dernier '/'
	size_t last_slash_pos = url.find_last_of('/');
	if (last_slash_pos == std::string::npos)
		filename = "unknown.bin";
	else
		filename = url.substr(last_slash_pos + 1);

	// 2. IMPORTANT : On coupe au '?' pour virer les paramètres
	size_t query_pos = filename.find('?');
	if (query_pos != std::string::npos)
		filename = filename.substr(0, query_pos);

	// 3. Si le nom est vide ou bizarre, on met un défaut
	if (filename.empty() || filename == ".")
		filename = "default.bin";

	return filename;
}