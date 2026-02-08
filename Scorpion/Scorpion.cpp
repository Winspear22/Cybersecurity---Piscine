/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scorpion.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:50:17 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 22:41:21 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scorpion.hpp"

Scorpion::Scorpion() {
  std::cout << MAGENTA << "Scorpion constructor called" << RESET << std::endl;
}

Scorpion::~Scorpion() {
  std::cout << MAGENTA << "Scorpion destructor called" << RESET << std::endl;
}

Scorpion::Scorpion(const Scorpion &src) {
  std::cout << MAGENTA << "Scorpion copy constructor called" << RESET
            << std::endl;
  *this = src;
}

Scorpion &Scorpion::operator=(const Scorpion &src) {
  std::cout << MAGENTA << "Scorpion copy assignment operator called" << RESET
            << std::endl;
  (void)src;
  return *this;
}

void Scorpion::FileIdentification(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << BOLD_RED << "Error: could not open " << filename << RESET
              << std::endl;
    return;
  }

  unsigned char headOfFile[12];
  file.read((char *)headOfFile, 12);
  std::string format = "Unknown";
  if (file.gcount() < 2) {
    std::cerr << RED << "File too small to be an image" << RESET << std::endl;
    return;
  }

  if (headOfFile[0] == 0xFF && headOfFile[1] == 0xD8)
    format = "JPEG";
  else if (headOfFile[0] == 0x89 && headOfFile[1] == 0x50)
    format = "PNG";
  else if (headOfFile[0] == 0x47 && headOfFile[1] == 0x49)
    format = "GIF";
  else if (headOfFile[0] == 0x42 && headOfFile[1] == 0x4D)
    format = "BMP";
  else if (headOfFile[0] == 0x52 && headOfFile[1] == 0x49) {
    if (headOfFile[8] == 'W' && headOfFile[9] == 'E' && headOfFile[10] == 'B' &&
        headOfFile[11] == 'P')
      format = "WEBP";
    else {
      std::cout << RED << "It's not a valid image file (.wev or .avi) !"
                << RESET << std::endl;
      return;
    }
  } else {
    std::cout << RED << "It's not a valid image file !" << RESET << std::endl;
    return;
  }
  std::cout << BOLD_WHITE << "Format de l'image : " << format << RESET
            << std::endl;
  this->_fileAnalysis(filename);
  if (format == "JPEG")
    this->_findExifBlock(file);
}

void Scorpion::_fileAnalysis(const std::string &fileName) {
  struct stat st;
  if (stat(fileName.c_str(), &st) == 0) {
    this->_dateAnalysis(st);
    this->_weightAnalysis(st);
  } else
    std::cerr << BOLD_RED << "Error: could not stat " << fileName << RESET
              << std::endl;
}

void Scorpion::_dateAnalysis(const struct stat &st) {
  char time_buf[20];
  strftime(time_buf, sizeof(time_buf), "%d/%m/%Y %H:%M",
           localtime(&st.st_mtime));
  std::cout << BOLD_YELLOW << "Date : " << RESET << time_buf << std::endl;
}

void Scorpion::_weightAnalysis(const struct stat &st) {
  std::cout << BOLD_YELLOW << "Weight : " << RESET << st.st_size << " bytes"
            << std::endl;
}

void Scorpion::_findExifBlock(std::ifstream &file)
{
  /*
     METHODE 1 : BRUTE FORCE (Active)
     Scanner chaque octet jusqu'à trouver FF E1.
  */

  file.seekg(2, std::ios::beg);
  char byte;
  while (file.get(byte))
  {
    if ((unsigned char)byte == 0xFF)
	{
      if (file.get(byte))
	  {
        if ((unsigned char)byte == 0xE1)
		{
          // On a trouvé le marker FF E1 !
          // Les 2 octets suivants sont la taille (on les ignore pour l'instant)
          file.seekg(2, std::ios::cur);

          char header[6];
          file.read(header, 6);
          if (std::string(header, 4) == "Exif")
		  {
            std::cout << BOLD_CYAN << "  [EXIF]   Found APP1 segment" << RESET << std::endl;
            this->_parseTiff(file);
            return;
          }
        }
		else if ((unsigned char)byte == 0xDA)
          // SOS : Start Of Scan = Début de l'image compressée
          break;
      }
    }
  }
  std::cout << YELLOW << "  [EXIF]   No metadata found" << RESET << std::endl;
}

/*
   METHODE 2 : SAUT DE SEGMENTS (Optimisée - À décommenter pour tester)
   Lire la taille des segments pour sauter par dessus.

void Scorpion::_findExifBlock_Optimized(std::ifstream &file)
{
  file.seekg(2, std::ios::beg);
  unsigned char marker[2];
  while (file.read((char*)marker, 2))
  {
    if (marker[0] != 0xFF) break;

    if (marker[1] == 0xE1)
    {
      unsigned char sizeBytes[2];
      file.read((char*)sizeBytes, 2);
      unsigned char exifHeader[6];
      file.read((char*)exifHeader, 6);
      if (std::string((char*)exifHeader, 4) == "Exif")
      {
        std::cout << BOLD_CYAN << "  [EXIF]   Found APP1 segment (Segment Jump)"
<< RESET << std::endl; this->_parseTiff(file); return;
      }
    }

    unsigned char sizeBytes[2];
    if (!file.read((char*)sizeBytes, 2)) break;

    unsigned short size = (sizeBytes[0] << 8) | sizeBytes[1];
    file.seekg(size - 2, std::ios::cur);

    if (marker[1] == 0xDA) break;
  }
  std::cout << YELLOW << "  [EXIF]   No metadata found" << RESET << std::endl;
}
*/

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
    std::cout << BOLD_CYAN << "  [TIFF]   Endianness : Intel (Little Endian)"
              << RESET << std::endl;
  } else if (endian[0] == 'M' && endian[1] == 'M') {
    isLittleEndian = false;
    std::cout << BOLD_CYAN << "  [TIFF]   Endianness : Motorola (Big Endian)"
              << RESET << std::endl;
  } else {
    std::cout << BOLD_RED << "  [TIFF]   Error: Invalid TIFF Header" << RESET
              << std::endl;
    return;
  }

  // ============================================
  // ÉTAPE 3 : Vérifier le Magic Number (42)
  // ============================================
  unsigned char magicNumber[2];
  file.read((char *)magicNumber, 2);

  bool magicOK = false;
  if (isLittleEndian) {
    // Little Endian : 42 = 2A 00
    if (magicNumber[0] == 0x2A && magicNumber[1] == 0x00)
      magicOK = true;
  } else {
    // Big Endian : 42 = 00 2A
    if (magicNumber[0] == 0x00 && magicNumber[1] == 0x2A)
      magicOK = true;
  }

  if (!magicOK) {
    std::cout << BOLD_RED << "  [TIFF]   Error: Magic Number is not 42" << RESET
              << std::endl;
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
  if (isLittleEndian) {
    // Little Endian : le premier octet est le plus petit
    ifdOffset = offsetBytes[0];
    ifdOffset = ifdOffset + (offsetBytes[1] * 256);
    ifdOffset = ifdOffset + (offsetBytes[2] * 256 * 256);
    ifdOffset = ifdOffset + (offsetBytes[3] * 256 * 256 * 256);
  } else {
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

  std::cout << BOLD_CYAN << "  [TIFF]   Number of Tags : " << numTags << RESET
            << std::endl;

  // ============================================
  // ÉTAPE 7 : Boucler sur les Tags (12 octets chacun)
  // ============================================
  unsigned short i = 0;
  while (i < numTags) {
    unsigned char tagEntry[12];
    file.read((char *)tagEntry, 12);

    // Les 2 premiers octets sont l'ID du Tag
    unsigned short tagID = 0;
    if (isLittleEndian)
      tagID = tagEntry[0] + (tagEntry[1] * 256);
    else
      tagID = tagEntry[1] + (tagEntry[0] * 256);

    // Affichage basique de l'ID
    // On peut ajouter des cas spéciaux pour les IDs connus
    if (tagID == 0x010F) // Manufacturer
      std::cout << GREEN << "  [TAG]    0x010F : Manufacturer" << RESET
                << std::endl;
    else if (tagID == 0x0110) // Model
      std::cout << GREEN << "  [TAG]    0x0110 : Camera Model" << RESET
                << std::endl;
    else if (tagID == 0x0132) // DateTime
      std::cout << GREEN << "  [TAG]    0x0132 : Date/Time" << RESET
                << std::endl;
    else if (tagID == 0x8769) // EXIF Sub-IFD
      std::cout << YELLOW
                << " [TAG]    0x8769 : EXIF Sub-IFD (more data inside)" << RESET
                << std::endl;
    else if (tagID == 0x8825) // GPS Info
      std::cout << YELLOW << " [TAG]    0x8825 : GPS Info" << RESET
                << std::endl;
    i = i + 1;
  }
  std::cout << BOLD_GREEN << " [TIFF]   Parsing complete!" << RESET << std::endl;
}