/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionPNG.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:02:41 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 15:16:26 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScorpionPNG.hpp"

ScorpionPNG::ScorpionPNG()
{
    std::cout << MAGENTA << "ScorpionPNG constructor called" << RESET
              << std::endl;
}

ScorpionPNG::~ScorpionPNG()
{
    std::cout << MAGENTA << "ScorpionPNG destructor called" << RESET
              << std::endl;
}

ScorpionPNG::ScorpionPNG(const ScorpionPNG &src) : Scorpion(src)
{
    std::cout << MAGENTA << "ScorpionPNG copy constructor called" << RESET
              << std::endl;
    *this = src;
}

ScorpionPNG &ScorpionPNG::operator=(const ScorpionPNG &src)
{
    std::cout << MAGENTA << "ScorpionPNG copy assignment operator called"
              << RESET << std::endl;
    (void)src;
    return *this;
}

void ScorpionPNG::parseSpecific(std::ifstream &file)
{
    // ============================================
    // ÉTAPE 1 : Sauter le header PNG (8 octets)
    // ============================================
    file.seekg(8, std::ios::beg);

    while (!file.eof())
    {
        // ============================================
        // ÉTAPE 2 : Lire la taille du chunk (4 octets Big Endian)
        // ============================================
        unsigned char sizeBytes[4];
        file.read((char *)sizeBytes, 4);
        if (file.gcount() < 4) break;

        unsigned long chunkSize = ((unsigned long)sizeBytes[0] << 24) |
                                  ((unsigned long)sizeBytes[1] << 16) |
                                  ((unsigned long)sizeBytes[2] << 8) |
                                  (unsigned long)sizeBytes[3];

        // ============================================
        // ÉTAPE 3 : Lire le type du chunk (4 octets ASCII)
        // ============================================
        unsigned char type[4];
        file.read((char *)type, 4);
        if (file.gcount() < 4) break;

        std::string chunkType((char *)type, 4);

        // ============================================
        // ÉTAPE 4 : Traiter selon le type de chunk
        // ============================================
        if (chunkType == "IHDR")
        {
            // Lire Width (4 octets Big Endian)
            unsigned char wBytes[4], hBytes[4];
            file.read((char *)wBytes, 4);
            file.read((char *)hBytes, 4);

            unsigned long width = ((unsigned long)wBytes[0] << 24) |
                                  ((unsigned long)wBytes[1] << 16) |
                                  ((unsigned long)wBytes[2] << 8) |
                                  (unsigned long)wBytes[3];
            unsigned long height = ((unsigned long)hBytes[0] << 24) |
                                   ((unsigned long)hBytes[1] << 16) |
                                   ((unsigned long)hBytes[2] << 8) |
                                   (unsigned long)hBytes[3];

            std::cout << GREEN << "  [PNG]    Dimensions : " << width << " x "
                      << height << RESET << std::endl;

            // Sauter le reste du chunk IHDR (13 - 8 = 5 octets restants) + CRC
            // (4)
            file.seekg(chunkSize - 8 + 4, std::ios::cur);
        }
        else if (chunkType == "eXIf")
        {
            // Le chunk eXIf contient directement les données TIFF (pas de
            // "Exif\0\0")
            std::cout << BOLD_CYAN << "  [EXIF]   Found eXIf chunk in PNG"
                      << RESET << std::endl;
            this->_parseTiff(file);
            // Après parseTiff on ne connait plus notre position, on sort
            return;
        }
        else if (chunkType == "IEND")
        {
            break;
        }
        else
        {
            // Sauter le contenu du chunk + CRC (4 octets)
            file.seekg(chunkSize + 4, std::ios::cur);
        }
    }
    std::cout << YELLOW << "  [EXIF]   No EXIF metadata found" << RESET
              << std::endl;
}
