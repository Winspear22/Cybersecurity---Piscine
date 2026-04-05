/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExifParserPNG.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:29:00 by adnen             #+#    #+#             */
/*   Updated: 2026/04/05 19:30:09 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExifParserPNG.hpp"

/* Forme canonique */
ExifParserPNG::ExifParserPNG()
{
	std::cout << "ExifParserPNG constructor called." << std::endl;
}

ExifParserPNG::~ExifParserPNG()
{
	std::cout << "ExifParserPNG destructor called." << std::endl;
}

ExifParserPNG::ExifParserPNG(const ExifParserPNG &other): ExifParser(other)
{
	*this = other;
	std::cout << "ExifParserPNG copy constructor called." << std::endl;
}

const ExifParserPNG &ExifParserPNG::operator=(const ExifParserPNG &other)
{
	if (this != &other)
		(void)other;
	std::cout << "ExifParserPNG copy assignment operator called." << std::endl;
	return *this;
}

/* ========================================================================== */
/*                         PARSING PNG                                       */
/* ========================================================================== */

/*
 * parse() — Extrait les métadonnées d'un fichier PNG.
 *
 * Structure PNG :
 *   [8] Signature PNG
 *   Puis des "chunks" :
 *     [4] longueur (big-endian)
 *     [4] type (4 chars ASCII : IHDR, tEXt, IEND...)
 *     [N] données
 *     [4] CRC
 *
 * IHDR (1er chunk) : largeur, hauteur, bit depth, color type
 * tEXt : keyword\0texte (Author, Comment, etc.)
 */
void ExifParserPNG::parse(const std::vector<unsigned char> &data)
{
	std::cout << "  --- Métadonnées PNG ---" << std::endl;
	
	/* Le 1er chunk commence après la signature PNG (8 octets) */
	size_t pos = 8;

	while (pos + 12 <= data.size()) // Un chunk = 4(len) + 4(type) + data + 4(CRC) = 12 octets minimum, donc on vérifie si le fichier est assez grand pour contenir un chunk
	{
    	/* Longueur du chunk (PNG = toujours big-endian) */
    	uint32_t chunkLen = _read32(data, pos, true); // On lit la longueur du chunk

    	/* Type du chunk (4 caractères) */
    	if (!_isOffsetSafe(data, pos + 4, 4)) // On vérifie si le fichier est assez grand pour contenir un chunk (ceinture + bretelles)
    		break;
    	std::string chunkType; // Après avoir eu la len, on lit le type du chunk
    	chunkType += static_cast<char>(data[pos + 4]); // On fait un static_cast pour convertir le caractère en string
    	chunkType += static_cast<char>(data[pos + 5]); // On cherche les 4 caractères qui composent le type du chunk
    	chunkType += static_cast<char>(data[pos + 6]); // Ces caractères sont toujours en ASCII
    	chunkType += static_cast<char>(data[pos + 7]); // Il s'agit de tEXt ou IHDR ou IEND, etc PAR CONTRE ON VEUT EVITER LE IDAT

    	size_t chunkDataStart = pos + 8;

    	/* ---- IHDR : dimensions de l'image ---- */
    	if (chunkType == "IHDR" && _isOffsetSafe(data, chunkDataStart, 13)) // IHDR vaut toujours 13 octets 
		{
      		uint32_t width = _read32(data, chunkDataStart, true); // 0-3 octets : largeur (4 octets)
      		uint32_t height = _read32(data, chunkDataStart + 4, true); // 4-7 octets : hauteur (4 octets)
      		uint8_t bitDepth = data[chunkDataStart + 8]; // 8 octets : bit depth (1 octet)
      		uint8_t colorType = data[chunkDataStart + 9]; // 9 octets : color type (1 octet)
			// On ignore le reste des octets du chunk IHDR (10-12) car ils sont toujoutd à 0 (il s'agit de Compression method, Filter method, Interlace method)

      		std::cout << "  Dimensions        : " << width << " x " << height << std::endl;
      		std::cout << "  Bit Depth         : " << static_cast<int>(bitDepth) << std::endl;

      		std::string colorStr = "Unknown";
      		if (colorType == 0)
        		colorStr = "Grayscale";
      		else if (colorType == 2)
        		colorStr = "RGB";
      		else if (colorType == 3)
        		colorStr = "Palette";
      		else if (colorType == 4)
        		colorStr = "Grayscale+Alpha";
      		else if (colorType == 6)
        		colorStr = "RGBA";
      		std::cout << "  Color Type        : " << colorStr << std::endl;
    	}

    	/* ---- tEXt : métadonnées textuelles ---- */
    	if (chunkType == "tEXt" && _isOffsetSafe(data, chunkDataStart, chunkLen))
		{
      		/* Format : keyword\0texte */
      		std::string keyword; // keyword est le nom de la métadonnée (Author, Comment, etc.)
      		size_t j = 0; // j est l'index pour parcourir le chunk tEXt
      		while (j < chunkLen) // On parcourt le chunk tEXt jusqu'à trouver le \0 (qui sépare le keyword du texte)
			{
        		if (data[chunkDataStart + j] == 0) // Si on trouve le \0, on sort de la boucle
          			break;
        		keyword += static_cast<char>(data[chunkDataStart + j]); // On ajoute le caractère à la keyword
        		j++; // On passe au caractère suivant
      		}
      		j++; /* sauter le \0 */

      		std::string textValue; // textValue est le texte qui suit le \0
      		while (j < chunkLen) // On parcourt le chunk tEXt jusqu'à la fin
			{
        		textValue += static_cast<char>(data[chunkDataStart + j]); // On ajoute le caractère à la textValue
        		j++; // On passe au caractère suivant
      		}

      		if (!keyword.empty()) // Si la keyword n'est pas vide
        		std::cout << "  " << std::left << std::setw(18) << keyword << ": " << textValue << std::endl; // On affiche la keyword et la textValue
    	}
		/* Exemple de sortie pour tEXt: 
			Author            : Adnen
  			Comment           : Super photo
  			Date              : 2026
		*/

    	/* IEND = fin du fichier */
    	if (chunkType == "IEND")
      		break;

    	/* Chunk suivant : 4(len) + 4(type) + data + 4(CRC) */
    	pos = pos + 12 + chunkLen;
	}
}
