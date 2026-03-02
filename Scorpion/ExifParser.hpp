/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExifParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:03:45 by adnen             #+#    #+#             */
/*   Updated: 2026/03/03 00:24:18 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __EXIFPARSER_HPP__
#define __EXIFPARSER_HPP__

#include "includes.hpp"

class ExifParser {
public:
  ExifParser();
  ~ExifParser();
  const ExifParser &operator=(const ExifParser &other);
  ExifParser(const ExifParser &other);

  /* Point d'entrée : affiche les données EXIF basiques d'un fichier */
  static void displayExifData(const std::string &filename);

private:
  /* Sécurité : vérifie qu'on ne lit pas hors du fichier */
  static bool _isOffsetSafe(const std::vector<unsigned char> &data,
                            size_t offset, size_t bytesNeeded);

  /* Charge le fichier en mémoire */
  static std::vector<unsigned char> _readFile(const std::string &filename);

  /* Trouve le début des données EXIF dans un JPEG */
  static int _findExifOffset(const std::vector<unsigned char> &data);

  /* Détecte l'ordre des octets : "MM" = big, "II" = little */
  static bool _isBigEndian(const std::vector<unsigned char> &data,
                           size_t offset);

  /* Lecture d'entiers avec gestion du byte order */
  static uint16_t _read16(const std::vector<unsigned char> &data, size_t offset,
                          bool bigEndian);
  static uint32_t _read32(const std::vector<unsigned char> &data, size_t offset,
                          bool bigEndian);

  /* Lecture d'une chaîne ASCII depuis les données binaires */
  static std::string _readAsciiTag(const std::vector<unsigned char> &data,
                                   size_t tiffStart, size_t entryValueOffset,
                                   uint32_t count, bool bigEndian);

  /* Parse un IFD et affiche les tags qu'on connaît */
  static void _parseIFD(const std::vector<unsigned char> &data,
                        size_t tiffStart, size_t ifdOffset, bool bigEndian,
                        bool isExifSubIFD);

  /* Parsing des métadonnées pour les autres formats */
  static void _parsePngMetadata(const std::vector<unsigned char> &data);
  static void _parseGifMetadata(const std::vector<unsigned char> &data);
  static void _parseBmpMetadata(const std::vector<unsigned char> &data);
};

#endif