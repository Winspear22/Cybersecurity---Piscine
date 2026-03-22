/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetadataEditor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 06:58:13 by adnen             #+#    #+#             */
/*   Updated: 2026/03/22 07:19:31 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MetadataEditor.hpp"

MetadataEditor::MetadataEditor()
{
    _filename = "";
    std::cout << "MetadataEditor constructor called" << std::endl;
}

MetadataEditor::~MetadataEditor()
{
    std::cout << "MetadataEditor destructor called" << std::endl;
}

MetadataEditor &MetadataEditor::operator=(const MetadataEditor &other)
{
    if (this != &other) _filename = other._filename;
    std::cout << "MetadataEditor copy assignment operator called" << std::endl;
    return (*this);
}

MetadataEditor::MetadataEditor(const MetadataEditor &src)
{
    std::cout << "MetadataEditor copy constructor called" << std::endl;
    *this = src;
}

const std::string &MetadataEditor::getFilename() const
{
    return (_filename);
}

void MetadataEditor::setFilename(const std::string &filename)
{
    _filename = filename;
}

bool MetadataEditor::deleteMetadata(const std::string &filename)
{
    // Identifier le format
    std::string format = MetadataEditor::setFormat(filename);
    if (format != "JPEG")
    {
        std::cerr << BOLD_RED << "Error: --delete only supports JPEG files"
                  << RESET << std::endl;
        return false;
    }

    // Charger tout le fichier dans un vector
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << BOLD_RED << "Error: could not open " << filename << RESET
                  << std::endl;
        return false;
    }
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
    file.close();

    // Reconstruire le fichier sans les segments APP1 (FF E1)
    // Même logique de scan que ScorpionJPEG::parseSpecific,
    // mais on COPIE au lieu de parser
    std::vector<unsigned char> output;

    // Copier le SOI (FF D8)
    output.push_back(data[0]);
    output.push_back(data[1]);

    size_t i = 2;
    while (i + 1 < data.size())
    {
        if (data[i] != 0xFF)
        {
            // Données brutes (après SOS), copier tout le reste
            output.insert(output.end(), data.begin() + i, data.end());
            break;
        }

        unsigned char marker = data[i + 1];

        // SOS (FF DA) = début des données compressées → copier tout le reste
        if (marker == 0xDA)
        {
            output.insert(output.end(), data.begin() + i, data.end());
            break;
        }

        // SOI (FF D8) et EOI (FF D9) n'ont pas de champ taille
        if (marker == 0xD8 || marker == 0xD9)
        {
            output.push_back(data[i]);
            output.push_back(data[i + 1]);
            i += 2;
            continue;
        }

        // Lire la taille du segment (2 octets, big endian)
        if (i + 3 >= data.size()) break;
        unsigned short segmentSize = (data[i + 2] * 256) + data[i + 3];
        size_t totalSegmentSize = 2 + segmentSize;

        // Si c'est un APP1 (FF E1) → on le SAUTE
        if (marker == 0xE1)
        {
            std::cout << GREEN << "  [DELETE] Skipping APP1 segment ("
                      << segmentSize << " bytes)" << RESET << std::endl;
            i += totalSegmentSize;
            continue;
        }

        // Sinon → on le COPIE
        if (i + totalSegmentSize > data.size()) break;
        output.insert(output.end(), data.begin() + i,
                      data.begin() + i + totalSegmentSize);
        i += totalSegmentSize;
    }

    // Réécrire le fichier
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
    {
        std::cerr << BOLD_RED << "Error: could not write " << filename << RESET
                  << std::endl;
        return false;
    }
    out.write((char *)output.data(), output.size());
    out.close();

    std::cout << BOLD_GREEN << "  ✅ EXIF metadata deleted from " << filename
              << RESET << std::endl;
    return true;
}

std::string MetadataEditor::getFormat(void) const
{
    return "";  // Désormais géré par setFormat statique si besoin
}

std::string MetadataEditor::setFormat(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << BOLD_RED << "Error: could not open " << filename << RESET
                  << std::endl;
        return "Error";
    }

    unsigned char headOfFile[12];
    file.read((char *)headOfFile, 12);
    std::string format = "Unknown";
    if (file.gcount() < 2)
    {
        std::cerr << RED << "File too small to be an image" << RESET
                  << std::endl;
        return format;
    }

    if (headOfFile[0] == 0xFF && headOfFile[1] == 0xD8)
        format = "JPEG";
    else if (headOfFile[0] == 0x89 && headOfFile[1] == 0x50)
        format = "PNG";
    else if (headOfFile[0] == 0x47 && headOfFile[1] == 0x49)
        format = "GIF";
    else if (headOfFile[0] == 0x42 && headOfFile[1] == 0x4D)
        format = "BMP";
    else if (headOfFile[0] == 0x52 && headOfFile[1] == 0x49)
    {
        if (headOfFile[8] == 'W' && headOfFile[9] == 'E' &&
            headOfFile[10] == 'B' && headOfFile[11] == 'P')
            format = "WEBP";
        else
        {
            std::cout << RED << "It's not a valid image file (.wev or .avi) !"
                      << RESET << std::endl;
            return format;
        }
    }
    else
    {
        std::cout << RED << "It's not a valid image file !" << RESET
                  << std::endl;
        return format;
    }
    return format;
}