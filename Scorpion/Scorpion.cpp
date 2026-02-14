/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scorpion.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:50:17 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 15:25:57 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scorpion.hpp"

#include "ScorpionBMP.hpp"
#include "ScorpionGIF.hpp"
#include "ScorpionJPEG.hpp"
#include "ScorpionPNG.hpp"
#include "ScorpionWEBP.hpp"

Scorpion::Scorpion()
{
    std::cout << MAGENTA << "Scorpion constructor called" << RESET << std::endl;
}

Scorpion::~Scorpion()
{
    std::cout << MAGENTA << "Scorpion destructor called" << RESET << std::endl;
}

Scorpion::Scorpion(const Scorpion &src)
{
    std::cout << MAGENTA << "Scorpion copy constructor called" << RESET
              << std::endl;
    *this = src;
}

Scorpion &Scorpion::operator=(const Scorpion &src)
{
    std::cout << MAGENTA << "Scorpion copy assignment operator called" << RESET
              << std::endl;
    (void)src;
    return *this;
}

void Scorpion::FileIdentification(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << BOLD_RED << "Error: could not open " << filename << RESET
                  << std::endl;
        return;
    }

    unsigned char headOfFile[12];
    file.read((char *)headOfFile, 12);
    std::string format = "Unknown";
    if (file.gcount() < 2)
    {
        std::cerr << RED << "File too small to be an image" << RESET
                  << std::endl;
        return;
    }

    Scorpion *parser = NULL;
    if (headOfFile[0] == 0xFF && headOfFile[1] == 0xD8)
    {
        format = "JPEG";
        parser = new ScorpionJPEG();
    }
    else if (headOfFile[0] == 0x89 && headOfFile[1] == 0x50)
    {
        format = "PNG";
        parser = new ScorpionPNG();
    }
    else if (headOfFile[0] == 0x47 && headOfFile[1] == 0x49)
    {
        format = "GIF";
        parser = new ScorpionGIF();
    }
    else if (headOfFile[0] == 0x42 && headOfFile[1] == 0x4D)
    {
        format = "BMP";
        parser = new ScorpionBMP();
    }
    else if (headOfFile[0] == 0x52 && headOfFile[1] == 0x49)
    {
        if (headOfFile[8] == 'W' && headOfFile[9] == 'E' &&
            headOfFile[10] == 'B' && headOfFile[11] == 'P')
        {
            format = "WEBP";
            parser = new ScorpionWEBP();
        }
        else
        {
            std::cout << RED << "It's not a valid image file (.wev or .avi) !"
                      << RESET << std::endl;
            return;
        }
    }
    else
    {
        std::cout << RED << "It's not a valid image file !" << RESET
                  << std::endl;
        return;
    }

    std::cout << BOLD_WHITE << "Format de l'image : " << format << RESET
              << std::endl;
    parser->_fileAnalysis(filename);
    parser->parseSpecific(file);
    delete parser;
}

void Scorpion::_fileAnalysis(const std::string &fileName)
{
    struct stat st;
    if (stat(fileName.c_str(), &st) == 0)
    {
        this->_dateAnalysis(st);
        this->_weightAnalysis(st);
    }
    else
        std::cerr << BOLD_RED << "Error: could not stat " << fileName << RESET
                  << std::endl;
}

void Scorpion::_dateAnalysis(const struct stat &st)
{
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%d/%m/%Y %H:%M",
             localtime(&st.st_mtime));
    std::cout << BOLD_YELLOW << "Date : " << RESET << time_buf << std::endl;
}

void Scorpion::_weightAnalysis(const struct stat &st)
{
    std::cout << BOLD_YELLOW << "Weight : " << RESET << st.st_size << " bytes"
              << std::endl;
}

void Scorpion::_parseTiff(std::ifstream &file)
{
    // ============================================
    // ÉTAPE 1 : Sauvegarder le début du bloc TIFF
    // ============================================
    long tiffStart = (long)file.tellg();

    // ============================================
    // ÉTAPE 2 : Lire l'Endianness (II ou MM)
    // ============================================
    unsigned char endian[2];
    file.read((char *)endian, 2);

    bool isLittleEndian = false;
    if (endian[0] == 'I' && endian[1] == 'I')
    {
        isLittleEndian = true;
        std::cout << BOLD_CYAN
                  << "  [TIFF]   Endianness : Intel (Little Endian)" << RESET
                  << std::endl;
    }
    else if (endian[0] == 'M' && endian[1] == 'M')
    {
        isLittleEndian = false;
        std::cout << BOLD_CYAN
                  << "  [TIFF]   Endianness : Motorola (Big Endian)" << RESET
                  << std::endl;
    }
    else
    {
        std::cout << BOLD_RED << "  [TIFF]   Error: Invalid TIFF Header"
                  << RESET << std::endl;
        return;
    }

    // ============================================
    // ÉTAPE 3 : Vérifier le Magic Number (42)
    // ============================================
    unsigned char magicNumber[2];
    file.read((char *)magicNumber, 2);

    bool magicOK = false;
    if (isLittleEndian)
    {
        // Little Endian : 42 = 2A 00
        if (magicNumber[0] == 0x2A && magicNumber[1] == 0x00) magicOK = true;
    }
    else
    {
        // Big Endian : 42 = 00 2A
        if (magicNumber[0] == 0x00 && magicNumber[1] == 0x2A) magicOK = true;
    }

    if (!magicOK)
    {
        std::cout << BOLD_RED << "  [TIFF]   Error: Magic Number is not 42"
                  << RESET << std::endl;
        return;
    }
    std::cout << BOLD_CYAN << "  [TIFF]   Magic Number : 42 (OK)" << RESET
              << std::endl;

    // ============================================
    // ÉTAPE 4 : Lire l'offset de l'IFD (4 octets)
    // ============================================
    unsigned char offsetBytes[4];
    file.read((char *)offsetBytes, 4);

    unsigned long ifdOffset = 0;
    if (isLittleEndian)
    {
        // Little Endian : le premier octet est le plus petit
        ifdOffset = offsetBytes[0];
        ifdOffset = ifdOffset + (offsetBytes[1] * 256);
        ifdOffset = ifdOffset + (offsetBytes[2] * 256 * 256);
        ifdOffset = ifdOffset + (offsetBytes[3] * 256 * 256 * 256);
    }
    else
    {
        // Big Endian : le premier octet est le plus grand
        ifdOffset = offsetBytes[3];
        ifdOffset = ifdOffset + (offsetBytes[2] * 256);
        ifdOffset = ifdOffset + (offsetBytes[1] * 256 * 256);
        ifdOffset = ifdOffset + (offsetBytes[0] * 256 * 256 * 256);
    }
    std::cout << BOLD_CYAN << "  [TIFF]   IFD Offset : " << ifdOffset << RESET
              << std::endl;

    // ============================================
    // ÉTAPE 5 : Aller à l'IFD
    // ============================================
    this->_parseIFD(file, tiffStart, isLittleEndian, ifdOffset);
}

void Scorpion::_parseIFD(std::ifstream &file, long tiffStart,
                         bool isLittleEndian, unsigned long ifdOffset)
{
    file.seekg(tiffStart + ifdOffset, std::ios::beg);

    // ============================================
    // ÉTAPE 6 : Lire le nombre de Tags (2 octets)
    // ============================================
    unsigned char numTagsBytes[2];
    file.read((char *)numTagsBytes, 2);

    unsigned short numTags = 0;
    if (isLittleEndian)
        numTags = numTagsBytes[0] + (numTagsBytes[1] * 256);
    else
        numTags = numTagsBytes[1] + (numTagsBytes[0] * 256);

    // ============================================
    // ÉTAPE 7 : Boucler sur les Tags (12 octets chacun)
    // ============================================
    unsigned short i = 0;
    while (i < numTags)
    {
        unsigned char tagEntry[12];
        file.read((char *)tagEntry, 12);

        // Appeler la fonction qui lit et affiche la valeur du tag
        this->_readTagValue(file, tagEntry, tiffStart, isLittleEndian);

        i = i + 1;
    }
}

void Scorpion::_readTagValue(std::ifstream &file, unsigned char *tagEntry,
                             long tiffStart, bool isLittleEndian)
{
    // Sauvegarder la position actuelle pour y revenir après
    long currentPos = file.tellg();

    // Lire l'ID du tag (octets 0-1)
    unsigned short tagID = 0;
    if (isLittleEndian)
        tagID = tagEntry[0] + (tagEntry[1] * 256);
    else
        tagID = tagEntry[1] + (tagEntry[0] * 256);

    // Lire le type de données (octets 2-3)
    unsigned short dataType = 0;
    if (isLittleEndian)
        dataType = tagEntry[2] + (tagEntry[3] * 256);
    else
        dataType = tagEntry[3] + (tagEntry[2] * 256);

    // Lire le nombre d'éléments (octets 4-7)
    unsigned long count = 0;
    if (isLittleEndian)
    {
        count = tagEntry[4];
        count = count + (tagEntry[5] * 256);
        count = count + (tagEntry[6] * 256 * 256);
        count = count + (tagEntry[7] * 256 * 256 * 256);
    }
    else
    {
        count = tagEntry[7];
        count = count + (tagEntry[6] * 256);
        count = count + (tagEntry[5] * 256 * 256);
        count = count + (tagEntry[4] * 256 * 256 * 256);
    }

    // Lire l'offset ou la valeur directe (octets 8-11)
    unsigned long valueOffset = 0;
    if (isLittleEndian)
    {
        valueOffset = tagEntry[8];
        valueOffset = valueOffset + (tagEntry[9] * 256);
        valueOffset = valueOffset + (tagEntry[10] * 256 * 256);
        valueOffset = valueOffset + (tagEntry[11] * 256 * 256 * 256);
    }
    else
    {
        valueOffset = tagEntry[11];
        valueOffset = valueOffset + (tagEntry[10] * 256);
        valueOffset = valueOffset + (tagEntry[9] * 256 * 256);
        valueOffset = valueOffset + (tagEntry[8] * 256 * 256 * 256);
    }

    // PRIORITÉ 1 : Vérifier si c'est un Sub-IFD ou GPS (peu importe le type)
    if (tagID == 0x8769)  // EXIF Sub-IFD
    {
        std::cout << YELLOW << "  [TAG]    0x8769 : EXIF Sub-IFD (Recursion...)"
                  << RESET << std::endl;
        this->_parseIFD(file, tiffStart, isLittleEndian, valueOffset);
    }
    else if (tagID == 0x8825)  // GPS Info
    {
        std::cout << YELLOW << "  [GPS]    Found GPS Info !" << RESET
                  << std::endl;
        this->_parseIFD(file, tiffStart, isLittleEndian, valueOffset);
    }
    // PRIORITÉ 2 : Traiter selon le type de données
    else if (dataType == 2)
    {
        // Pour le texte, aller lire à l'offset
        file.seekg(tiffStart + valueOffset, std::ios::beg);

        char textBuffer[256];
        file.read(textBuffer, count);
        textBuffer[count] = '\0';  // Terminer la chaîne

        // Afficher selon l'ID du tag
        if (tagID == 0x010F)
            std::cout << GREEN << "  [TAG]    Manufacturer : " << textBuffer
                      << RESET << std::endl;
        else if (tagID == 0x0110)
            std::cout << GREEN << "  [TAG]    Camera Model : " << textBuffer
                      << RESET << std::endl;
        else if (tagID == 0x0132)
            std::cout << GREEN << "  [TAG]    Date/Time : " << textBuffer
                      << RESET << std::endl;
    }
    else if (dataType == 5)  // RATIONAL (2x Long : Numérateur / Dénominateur)
    {
        // On va lire les 3 valeurs (Degrés, Minutes, Secondes) à l'offset
        if (tagID == 0x0002 || tagID == 0x0004)  // Latitude ou Longitude
        {
            file.seekg(tiffStart + valueOffset, std::ios::beg);
            unsigned int degreesNum, degreesDenom;
            unsigned int minutesNum, minutesDenom;
            unsigned int secondsNum, secondsDenom;

            // Lire Degrés
            file.read((char *)&degreesNum, 4);
            file.read((char *)&degreesDenom, 4);
            // Lire Minutes
            file.read((char *)&minutesNum, 4);
            file.read((char *)&minutesDenom, 4);
            // Lire Secondes
            file.read((char *)&secondsNum, 4);
            file.read((char *)&secondsDenom, 4);

            // Gérer l'endianness
            if (!isLittleEndian)
            {
                degreesNum = __builtin_bswap32(degreesNum);
                degreesDenom = __builtin_bswap32(degreesDenom);
                minutesNum = __builtin_bswap32(minutesNum);
                minutesDenom = __builtin_bswap32(minutesDenom);
                secondsNum = __builtin_bswap32(secondsNum);
                secondsDenom = __builtin_bswap32(secondsDenom);
            }

            double deg = (double)degreesNum / degreesDenom;
            double min = (double)minutesNum / minutesDenom;
            double sec = (double)secondsNum / secondsDenom;

            if (tagID == 0x0002)
                std::cout << BOLD_MAGENTA << "  [GPS]    Latitude  : " << deg
                          << " deg " << min << "' " << sec << "\"" << RESET
                          << std::endl;
            else
                std::cout << BOLD_MAGENTA << "  [GPS]    Longitude : " << deg
                          << " deg " << min << "' " << sec << "\"" << RESET
                          << std::endl;
        }
    }

    // Revenir à la position d'origine
    file.seekg(currentPos, std::ios::beg);
}