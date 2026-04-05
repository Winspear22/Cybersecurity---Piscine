/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileValidator.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 23:10:01 by adnen             #+#    #+#             */
/*   Updated: 2026/04/05 15:00:10 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileValidator.hpp"

FileValidator::FileValidator()
{
	std::cout << "FileValidator constructor called." << std::endl;
}

FileValidator::~FileValidator()
{
	std::cout << "FileValidator destructor called." << std::endl;
}

const FileValidator &FileValidator::operator=(const FileValidator &other)
{
	if (this != &other) {}
	std::cout << "FileValidator copy assignment operator called." << std::endl;
	return *this;
}

FileValidator::FileValidator(const FileValidator &other)
{
	*this = other;
	std::cout << "FileValidator copy constructor called." << std::endl;
}

bool FileValidator::isFileOfValidFormat(const std::string &filename)
{
	std::filesystem::path path(filename);
	
	// 2. Rendre le programme insensible à la casse : image.JPG --> image.jpg
	std::string ext = path.extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c)
	{
		return std::tolower(c); 
	});

	// 3. Ouverture en binaire
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open " << filename << std::endl;
		return FAILURE;
	}

	// 4. Lecture du header
	unsigned char header[8] = {0}; // unsigned char est un entier non signé de 8 bits, le binaire n'est pas négatif
	file.read(reinterpret_cast<char *>(header), 8); // reinterpret_cast est utilisé pour convertir le pointeur en char* car read attend un char*
	size_t bytesRead = file.gcount(); // gcount() retourne le nombre d'octets lus réellement (si le fichier est trop petit, ce n'est pas une image)

	// 5. Détection du format réel (Magic Numbers)
	std::string actualFormat = "UNKNOWN";
	if (bytesRead >= 3 && header[0] == 0xFF && header[1] == 0xD8 && header[2] == 0xFF) // FF D8 FF = JPEG --> FF est un marqueur de début de segment en JPEG
		actualFormat = "JPEG";
	else if (bytesRead >= 4 && header[0] == 0x89 && header[1] == 0x50 && header[2] == 0x4E && header[3] == 0x47) /*89 = octet spécial non ASCII pour vérifier que l'image n'est pas corrompue 50 = P 4E = N 47 = G*/
		actualFormat = "PNG";
	else if (bytesRead >= 4 && header[0] == 0x47 && header[1] == 0x49 && header[2] == 0x46 && header[3] == 0x38) /*47 = G 49 = I 46 = F 38 = 8 on rajoute 3941 pour faire 9a ça fait GIF89a*/
		actualFormat = "GIF";
	else if (bytesRead >= 2 && header[0] == 0x42 && header[1] == 0x4D) /*42 = B 4D = M pour BITMAP*/
		actualFormat = "BMP";

	if (actualFormat == "UNKNOWN")
	{
		std::cerr << "Error: " << filename << " format not supported or corrupted." << std::endl;
		return FAILURE;
	}

	// 6. Cross-validation (Magic vs Ext) - "Béton armé"
	if (actualFormat == "JPEG" && (ext == ".jpg" || ext == ".jpeg"))
		return SUCCESS;
	if (actualFormat == "PNG" && ext == ".png")
		return SUCCESS;
	if (actualFormat == "GIF" && ext == ".gif")
    	return SUCCESS;
	if (actualFormat == "BMP" && ext == ".bmp")
    	return SUCCESS;

	// En cas de mismatch (ex: un .png renommé en .jpg)
	std::string displayExt = ext;
	if (ext.empty())
		displayExt = "none";
	std::cerr << "Error: Mismatch detected for " << filename << " (Extension is " << displayExt << " but file is actually " << actualFormat << ")." << std::endl;
	return FAILURE;
}