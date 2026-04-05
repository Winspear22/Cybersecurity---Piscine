/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExifParserGIF.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:29:00 by adnen             #+#    #+#             */
/*   Updated: 2026/04/05 18:56:14 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExifParserGIF.hpp"

ExifParserGIF::ExifParserGIF()
{
	std::cout << "ExifParserGIF constructor" << std::endl;
}

ExifParserGIF::~ExifParserGIF()
{
	std::cout << "ExifParserGIF destructor" << std::endl;
}

ExifParserGIF::ExifParserGIF(const ExifParserGIF &other): ExifParser(other)
{
	std::cout << "ExifParserGIF copy constructor" << std::endl;
	*this = other;
}

const ExifParserGIF &ExifParserGIF::operator=(const ExifParserGIF &other)
{
	std::cout << "ExifParserGIF assignment operator" << std::endl;
	if (this != &other)
		(void)other;
	return *this;
}

/* ========================================================================== */
/*                         PARSING GIF                                       */
/* ========================================================================== */

/*
 * parse() — Extrait les métadonnées d'un fichier GIF.
 *
 * Structure du header GIF :
 *   [6] Signature : "GIF87a" ou "GIF89a"
 *   [2] Largeur  (little-endian)
 *   [2] Hauteur  (little-endian)
 *   [1] Packed byte :
 *       bit 7     = palette globale présente
 *       bits 0-2  = taille de la palette (2^(n+1) couleurs)
 */
void ExifParserGIF::parse(const std::vector<unsigned char> &data)
{
	std::cout << "  --- Métadonnées GIF ---" << std::endl;

	if (!_isOffsetSafe(data, 0, 13)) // Vérifie si le fichier est assez grand pour contenir le header GIF
	{
		std::cerr << "  Fichier GIF trop court." << std::endl;
		return;
	}

	/* Version (GIF87a ou GIF89a) */
	std::string version; // Version du fichier GIF
	size_t i = 0; // Index pour parcourir le header GIF
	while (i < 6) // On parcourt les 6 premiers octets du header GIF
	{
		version += static_cast<char>(data[i]); // Ajoute le caractère à la version
		i++;
	}
	std::cout << "  Version           : " << version << std::endl;

	/* Dimensions (little-endian) */
	uint16_t width = _read16(data, 6, false); // Lecture de la largeur du fichier GIF
	uint16_t height = _read16(data, 8, false); // Lecture de la hauteur du fichier GIF
	std::cout << "  Dimensions        : " << width << " x " << height << std::endl;

	/* Palette globale (palette de couleurs), AVANT la mémoire était très chère donc on stock les couleurs dans une palette avec des chiffres de 0 à 255 
	   et on utilise ces chiffres pour afficher les couleurs (genre rouge = 1, vert = 2, bleu = 3, etc) */
	/* On a lu le header du GIF qui fait 6 octets, on a lu les dimensions du GIF qui font 4 octets, on lit mtn l'octet packed qui fait 1 octet (data[10])*/
	uint8_t packed = data[10]; // On veut savoir s'il y'a une palette globale (palette de couleurs), c'est cet octet qui nous le dit
	bool hasGlobalPalette = false; 
	if (packed >= 128) // On regarde l'octet n°10. S'il vaut 128 ou plus, c'est que le fichier contient une liste de couleurs.
		hasGlobalPalette = true;
	if (hasGlobalPalette) // Notre 10e bit est composé de 8 octets : 7e octet = y'a-t-il une palette ? 4-5-6 : info sur la résolution 3 : info sur le tri des couleurs 2 : y'a-t-il un tri de couleurs ? 1-2 : info sur la palette globale
	{					  // et enfin 0-1-2 : information sur la TAILLE de la palette globale
    	int colorBits = (packed & 0x07) + 1; // Cette ligne isole les trois derniers bits de l'octet pour calculer sur combien de bits sont codées les couleurs de l'image.
    	int numColors = 1;
    	int k = 0;
    	while (k < colorBits)
		{
      		numColors = numColors * 2; // Si on a 1 bit de couleur alors on a 2^1 couleurs, si 4 bits alors 2^4 couleurs, etc jusqu'à 256 couleurs (2^8)
      		k++;
    	}
    	std::cout << "  Couleurs          : " << numColors << std::endl;
	}
}