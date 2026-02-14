/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionWEBP.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:39:00 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 15:17:00 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScorpionWEBP.hpp"

ScorpionWEBP::ScorpionWEBP()
{
    std::cout << MAGENTA << "ScorpionWEBP constructor called" << RESET
              << std::endl;
}

ScorpionWEBP::~ScorpionWEBP()
{
    std::cout << MAGENTA << "ScorpionWEBP destructor called" << RESET
              << std::endl;
}

ScorpionWEBP::ScorpionWEBP(const ScorpionWEBP &src) : Scorpion(src)
{
    std::cout << MAGENTA << "ScorpionWEBP copy constructor called" << RESET
              << std::endl;
    *this = src;
}

ScorpionWEBP &ScorpionWEBP::operator=(const ScorpionWEBP &src)
{
    std::cout << MAGENTA << "ScorpionWEBP copy assignment operator called"
              << RESET << std::endl;
    (void)src;
    return *this;
}

void ScorpionWEBP::parseSpecific(std::ifstream &file)
{
    // ============================================
    // ÉTAPE 1 : Sauter le RIFF header (12 octets)
    //           "RIFF" (4) + File Size (4) + "WEBP" (4)
    // ============================================
    file.seekg(12, std::ios::beg);

    bool foundDimensions = false;
    bool foundExif = false;

    while (!file.eof())
    {
        // ============================================
        // ÉTAPE 2 : Lire le FourCC du sous-chunk (4 octets)
        // ============================================
        unsigned char fourCC[4];
        file.read((char *)fourCC, 4);
        if (file.gcount() < 4) break;

        std::string chunkID((char *)fourCC, 4);

        // ============================================
        // ÉTAPE 3 : Lire la taille du sous-chunk (4 octets Little Endian)
        // ============================================
        unsigned char sizeBytes[4];
        file.read((char *)sizeBytes, 4);
        if (file.gcount() < 4) break;

        unsigned long chunkSize = sizeBytes[0] + (sizeBytes[1] * 256) +
                                  (sizeBytes[2] * 256 * 256) +
                                  (sizeBytes[3] * 256 * 256 * 256);

        // ============================================
        // ÉTAPE 4 : Traiter selon le type de chunk
        // ============================================
        if (chunkID == "VP8 " && !foundDimensions)
        {
            // VP8 lossy : les dimensions sont à l'offset +6 dans le bitstream
            // Sauter 3 octets (frame tag) + 3 octets (sync code 9D 01 2A)
            file.seekg(6, std::ios::cur);

            unsigned char wBytes[2], hBytes[2];
            file.read((char *)wBytes, 2);
            file.read((char *)hBytes, 2);

            // Les 14 bits de poids faible contiennent la dimension
            unsigned short width = (wBytes[0] | (wBytes[1] << 8)) & 0x3FFF;
            unsigned short height = (hBytes[0] | (hBytes[1] << 8)) & 0x3FFF;

            std::cout << GREEN << "  [WEBP]   Dimensions : " << width << " x "
                      << height << RESET << std::endl;
            foundDimensions = true;

            // Revenir à la fin du chunk
            file.seekg(chunkSize - 10, std::ios::cur);
        }
        else if (chunkID == "VP8L" && !foundDimensions)
        {
            // VP8L lossless : 1 octet signature (0x2F) + 4 octets contenant w/h
            file.seekg(1, std::ios::cur);

            unsigned char data[4];
            file.read((char *)data, 4);

            unsigned long bits =
                data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
            unsigned short width = (bits & 0x3FFF) + 1;
            unsigned short height = ((bits >> 14) & 0x3FFF) + 1;

            std::cout << GREEN << "  [WEBP]   Dimensions : " << width << " x "
                      << height << RESET << std::endl;
            foundDimensions = true;

            file.seekg(chunkSize - 5, std::ios::cur);
        }
        else if (chunkID == "VP8X" && !foundDimensions)
        {
            // VP8X extended : flags (4 octets) + 3 octets width + 3 octets
            // height
            file.seekg(4, std::ios::cur);

            unsigned char wBytes[3], hBytes[3];
            file.read((char *)wBytes, 3);
            file.read((char *)hBytes, 3);

            unsigned long width =
                (wBytes[0] | (wBytes[1] << 8) | (wBytes[2] << 16)) + 1;
            unsigned long height =
                (hBytes[0] | (hBytes[1] << 8) | (hBytes[2] << 16)) + 1;

            std::cout << GREEN << "  [WEBP]   Dimensions : " << width << " x "
                      << height << RESET << std::endl;
            foundDimensions = true;

            file.seekg(chunkSize - 10, std::ios::cur);
        }
        else if (chunkID == "EXIF")
        {
            std::cout << BOLD_CYAN << "  [EXIF]   Found EXIF chunk in WEBP"
                      << RESET << std::endl;
            // Le chunk EXIF peut commencer par "Exif\0\0" (6 octets) ou
            // directement par le header TIFF
            char prefix[6];
            long exifStart = file.tellg();
            file.read(prefix, 6);

            if (std::string(prefix, 4) == "Exif")
            {
                // Sauter le prefix "Exif\0\0"
                this->_parseTiff(file);
            }
            else
            {
                // Pas de prefix, revenir au début et parser directement
                file.seekg(exifStart, std::ios::beg);
                this->_parseTiff(file);
            }
            foundExif = true;
            return;
        }
        else
        {
            // Sauter le chunk inconnu
            file.seekg(chunkSize, std::ios::cur);
        }

        // Les chunks RIFF doivent être alignés sur 2 octets
        if (chunkSize % 2 != 0) file.seekg(1, std::ios::cur);
    }

    if (!foundExif)
        std::cout << YELLOW << "  [EXIF]   No EXIF metadata found" << RESET
                  << std::endl;
}
