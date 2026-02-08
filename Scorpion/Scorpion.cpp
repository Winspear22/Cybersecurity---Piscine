/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scorpion.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:50:17 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 21:33:09 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scorpion.hpp"

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
    std::cerr << BOLD_RED << "Error: could not open " << filename << RESET << std::endl;
    return;
  }

  unsigned char headOfFile[12];
  file.read((char *)headOfFile, 12);
  std::string format = "Unknown";
  if (file.gcount() < 2)
  {
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
  else if (headOfFile[0] == 0x52 && headOfFile[1] == 0x49)
  {
    if (headOfFile[8] == 'W' && headOfFile[9] == 'E' && headOfFile[10] == 'B' && headOfFile[11] == 'P')
      format = "WEBP";
    else
	{
      std::cout << RED << "It's not a valid image file (.wev or .avi) !" << RESET << std::endl;
      return;
    }
  }
  else
  {
    std::cout << RED << "It's not a valid image file !" << RESET << std::endl;
    return;
  }
  std::cout << BOLD_WHITE << "Format de l'image : " << format << RESET
            << std::endl;
  this->_fileAnalysis(filename);
  if (format == "JPEG")
    this->_findExifBlock(file);
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
    std::cerr << BOLD_RED << "Error: could not stat " << fileName << RESET << std::endl;
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
            std::cout << BOLD_CYAN
                      << "  [EXIF]   Found APP1 segment (Brute Force)" << RESET
                      << std::endl;
            // TODO: Lancer le parsing TIFF ici
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

  /*
     METHODE 2 : SAUT DE SEGMENTS (Optimisée - À décommenter pour tester)
     Lire la taille des segments pour sauter par dessus.

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
                          std::cout << BOLD_CYAN << "  [EXIF]   Found APP1
  segment (Segment Jump)" << RESET << std::endl;
                          // TODO: _parseTiff(file);
                          return;
                  }
          }

          // Lire la taille pour sauter
          unsigned char sizeBytes[2];
          if (!file.read((char*)sizeBytes, 2)) break;

          unsigned short size = (sizeBytes[0] << 8) | sizeBytes[1];
          file.seekg(size - 2, std::ios::cur);

          if (marker[1] == 0xDA) break;
  }
  std::cout << YELLOW << "  [EXIF]   No metadata found" << RESET << std::endl;
  */
}