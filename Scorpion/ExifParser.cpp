/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExifParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:03:42 by adnen             #+#    #+#             */
/*   Updated: 2026/03/03 00:25:20 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExifParser.hpp"

ExifParser::ExifParser() {
  std::cout << "ExifParser constructor called." << std::endl;
}

ExifParser::~ExifParser() {
  std::cout << "ExifParser destructor called." << std::endl;
}

const ExifParser &ExifParser::operator=(const ExifParser &other) {
  if (this != &other) {
  }
  std::cout << "ExifParser copy assignment operator called." << std::endl;
  return *this;
}

ExifParser::ExifParser(const ExifParser &other) {
  *this = other;
  std::cout << "ExifParser copy constructor called." << std::endl;
}

/* ========================================================================== */
/*                          FONCTIONS DE SÉCURITÉ                            */
/* ========================================================================== */

/*
 * _isOffsetSafe() — Empêche de lire au-delà du fichier.
 * On vérifie TOUJOURS avant chaque lecture binaire.
 */
bool ExifParser::_isOffsetSafe(const std::vector<unsigned char> &data,
                               size_t offset, size_t bytesNeeded) {
  if (offset + bytesNeeded < offset) /* overflow */
    return false;
  if (offset + bytesNeeded > data.size())
    return false;
  return true;
}

/* ========================================================================== */
/*                         LECTURE DU FICHIER                                */
/* ========================================================================== */

/*
 * _readFile() — Lit tout le fichier en mémoire (max 50 Mo).
 */
std::vector<unsigned char> ExifParser::_readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (!file.is_open())
    return std::vector<unsigned char>();

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  if (size <= 0 || size > 50 * 1024 * 1024)
    return std::vector<unsigned char>();

  std::vector<unsigned char> data(size);
  file.read(reinterpret_cast<char *>(data.data()), size);
  return data;
}

/* ========================================================================== */
/*                  RECHERCHE DU SEGMENT EXIF DANS LE JPEG                   */
/* ========================================================================== */

/*
 * _findExifOffset() — Cherche le marqueur APP1 (FF E1) + "Exif\0\0".
 * Retourne la position du header TIFF, ou -1 si pas d'EXIF.
 */
int ExifParser::_findExifOffset(const std::vector<unsigned char> &data) {
  /* Un JPEG commence toujours par FF D8 */
  if (!_isOffsetSafe(data, 0, 2))
    return -1;
  if (data[0] != 0xFF || data[1] != 0xD8)
    return -1;

  size_t pos = 2;

  while (pos + 4 < data.size()) {
    if (data[pos] != 0xFF)
      return -1;

    uint8_t marker = data[pos + 1];

    /* SOS = fin des métadonnées, on arrête */
    if (marker == 0xDA)
      return -1;

    if (!_isOffsetSafe(data, pos + 2, 2))
      return -1;
    uint16_t segLen = (data[pos + 2] << 8) | data[pos + 3];

    /* APP1 (0xE1) = le segment EXIF */
    if (marker == 0xE1 && _isOffsetSafe(data, pos + 4, 6)) {
      if (data[pos + 4] == 'E' && data[pos + 5] == 'x' &&
          data[pos + 6] == 'i' && data[pos + 7] == 'f' &&
          data[pos + 8] == 0x00 && data[pos + 9] == 0x00) {
        return (int)(pos + 10);
      }
    }

    pos = pos + 2 + segLen;
  }
  return -1;
}

/* ========================================================================== */
/*                        LECTURE AVEC BYTE ORDER                            */
/* ========================================================================== */

/* "MM" = Big Endian (Motorola), "II" = Little Endian (Intel) */
bool ExifParser::_isBigEndian(const std::vector<unsigned char> &data,
                              size_t offset) {
  if (!_isOffsetSafe(data, offset, 2))
    return false;
  return (data[offset] == 'M' && data[offset + 1] == 'M');
}

/* Lit 2 octets dans le bon ordre */
uint16_t ExifParser::_read16(const std::vector<unsigned char> &data,
                             size_t offset, bool bigEndian) {
  if (!_isOffsetSafe(data, offset, 2))
    return 0;
  if (bigEndian)
    return (data[offset] << 8) | data[offset + 1];
  return data[offset] | (data[offset + 1] << 8);
}

/* Lit 4 octets dans le bon ordre */
uint32_t ExifParser::_read32(const std::vector<unsigned char> &data,
                             size_t offset, bool bigEndian) {
  if (!_isOffsetSafe(data, offset, 4))
    return 0;
  if (bigEndian) {
    return ((uint32_t)data[offset] << 24) | ((uint32_t)data[offset + 1] << 16) |
           ((uint32_t)data[offset + 2] << 8) | (uint32_t)data[offset + 3];
  }
  return (uint32_t)data[offset] | ((uint32_t)data[offset + 1] << 8) |
         ((uint32_t)data[offset + 2] << 16) |
         ((uint32_t)data[offset + 3] << 24);
}

/* ========================================================================== */
/*                     LECTURE D'UN TAG DE TYPE ASCII                         */
/* ========================================================================== */

/*
 * _readAsciiTag() — Lit un tag de type ASCII (type 2).
 * Si la donnée tient dans 4 octets → inline dans l'entrée IFD.
 * Sinon → c'est un offset vers les données.
 */
std::string ExifParser::_readAsciiTag(const std::vector<unsigned char> &data,
                                      size_t tiffStart, size_t entryValueOffset,
                                      uint32_t count, bool bigEndian) {
  size_t dataOffset;

  /* Si le texte tient dans 4 octets, il est stocké sur place */
  if (count <= 4)
    dataOffset = entryValueOffset;
  else {
    /* Sinon, les 4 octets contiennent un offset */
    uint32_t rawOffset = _read32(data, entryValueOffset, bigEndian);
    dataOffset = tiffStart + rawOffset;
  }

  if (!_isOffsetSafe(data, dataOffset, count))
    return "";

  /* Lire les caractères un par un */
  std::string result;
  size_t i = 0;
  while (i < count) {
    if (data[dataOffset + i] == 0)
      break;
    result += static_cast<char>(data[dataOffset + i]);
    i++;
  }
  return result;
}

/* ========================================================================== */
/*                      PARSING D'UN IFD (SIMPLIFIÉ)                         */
/* ========================================================================== */

/*
 * _parseIFD() — Parcourt un IFD et affiche UNIQUEMENT les tags utiles :
 *
 * Dans IFD0 :
 *   0x010F = Make           (marque de l'appareil)
 *   0x0110 = Model          (modèle)
 *   0x0131 = Software       (logiciel d'édition)
 *   0x0132 = DateTime       (date de modification)
 *   0x8769 = ExifIFDPointer (pointe vers le sous-IFD Exif)
 *
 * Dans Exif Sub-IFD :
 *   0x9003 = DateTimeOriginal  (date de prise de vue)
 *   0x9004 = DateTimeDigitized (date de numérisation)
 */
void ExifParser::_parseIFD(const std::vector<unsigned char> &data,
                           size_t tiffStart, size_t ifdOffset, bool bigEndian,
                           bool isExifSubIFD) {
  size_t absOffset = tiffStart + ifdOffset;

  if (!_isOffsetSafe(data, absOffset, 2))
    return;

  uint16_t entryCount = _read16(data, absOffset, bigEndian);

  /* Protection : max 500 entrées */
  if (entryCount > 500)
    return;

  size_t entryBase = absOffset + 2;
  uint16_t idx = 0;

  while (idx < entryCount) {
    size_t entryPos = entryBase + idx * 12;

    if (!_isOffsetSafe(data, entryPos, 12))
      break;

    uint16_t tag = _read16(data, entryPos, bigEndian);
    uint16_t type = _read16(data, entryPos + 2, bigEndian);
    uint32_t count = _read32(data, entryPos + 4, bigEndian);

    /* On ne traite QUE les tags ASCII (type 2) qu'on connaît */
    if (type == 2) {
      std::string value =
          _readAsciiTag(data, tiffStart, entryPos + 8, count, bigEndian);

      if (!value.empty()) {
        /* IFD0 : infos de l'appareil */
        if (!isExifSubIFD) {
          if (tag == 0x010F)
            std::cout << "  Make              : " << value << std::endl;
          if (tag == 0x0110)
            std::cout << "  Model             : " << value << std::endl;
          if (tag == 0x0131)
            std::cout << "  Software          : " << value << std::endl;
          if (tag == 0x0132)
            std::cout << "  DateTime          : " << value << std::endl;
          if (tag == 0x013B)
            std::cout << "  Artist            : " << value << std::endl;
          if (tag == 0x8298)
            std::cout << "  Copyright         : " << value << std::endl;
        }
        /* Exif Sub-IFD : dates de prise de vue */
        if (isExifSubIFD) {
          if (tag == 0x9003)
            std::cout << "  DateTimeOriginal  : " << value << std::endl;
          if (tag == 0x9004)
            std::cout << "  DateTimeDigitized : " << value << std::endl;
        }
      }
    }

    /* Tag spécial : pointeur vers le sous-IFD Exif */
    if (tag == 0x8769 && !isExifSubIFD) {
      uint32_t subOffset = _read32(data, entryPos + 8, bigEndian);
      _parseIFD(data, tiffStart, subOffset, bigEndian, true);
    }

    idx++;
  }
}

/* ========================================================================== */
/*                         PARSING PNG (IHDR + tEXt)                         */
/* ========================================================================== */

/*
 * _parsePngMetadata() — Extrait les métadonnées d'un fichier PNG.
 *
 * Structure PNG :
 *   8 octets  → signature PNG
 *   Puis des "chunks" :
 *     [4] longueur  [4] type  [N] données  [4] CRC
 *
 * IHDR (toujours le 1er chunk) :
 *   [4] largeur  [4] hauteur  [1] bitDepth  [1] colorType
 *
 * tEXt :
 *   keyword\0texte (métadonnées textuelles : Author, Comment, etc.)
 */
void ExifParser::_parsePngMetadata(const std::vector<unsigned char> &data) {
  /* La signature PNG fait 8 octets, le 1er chunk commence à l'offset 8 */
  size_t pos = 8;

  std::cout << "  --- Métadonnées PNG ---" << std::endl;

  while (pos + 12 <= data.size()) {
    /* Longueur du chunk (big-endian, toujours en PNG) */
    uint32_t chunkLen = _read32(data, pos, true);

    /* Type du chunk (4 caractères ASCII) */
    if (!_isOffsetSafe(data, pos + 4, 4))
      break;
    std::string chunkType;
    chunkType += (char)data[pos + 4];
    chunkType += (char)data[pos + 5];
    chunkType += (char)data[pos + 6];
    chunkType += (char)data[pos + 7];

    size_t chunkDataStart = pos + 8;

    /* IHDR : dimensions de l'image */
    if (chunkType == "IHDR" && _isOffsetSafe(data, chunkDataStart, 13)) {
      uint32_t width = _read32(data, chunkDataStart, true);
      uint32_t height = _read32(data, chunkDataStart + 4, true);
      uint8_t bitDepth = data[chunkDataStart + 8];
      uint8_t colorType = data[chunkDataStart + 9];

      std::cout << "  Dimensions        : " << width << " x " << height
                << std::endl;
      std::cout << "  Bit Depth         : " << (int)bitDepth << std::endl;

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

    /* tEXt : métadonnées textuelles (Author, Comment, etc.) */
    if (chunkType == "tEXt" && _isOffsetSafe(data, chunkDataStart, chunkLen)) {
      /* Format : keyword\0texte */
      std::string keyword;
      size_t j = 0;
      while (j < chunkLen) {
        if (data[chunkDataStart + j] == 0)
          break;
        keyword += (char)data[chunkDataStart + j];
        j++;
      }
      j++; /* sauter le \0 */
      std::string textValue;
      while (j < chunkLen) {
        textValue += (char)data[chunkDataStart + j];
        j++;
      }
      if (!keyword.empty())
        std::cout << "  " << std::left << std::setw(18) << keyword << ": "
                  << textValue << std::endl;
    }

    /* IEND = fin du fichier PNG */
    if (chunkType == "IEND")
      break;

    /* Passer au chunk suivant : longueur + 4(len) + 4(type) + 4(CRC) */
    pos = pos + 12 + chunkLen;
  }
}

/* ========================================================================== */
/*                           PARSING GIF                                     */
/* ========================================================================== */

/*
 * _parseGifMetadata() — Extrait les métadonnées d'un fichier GIF.
 *
 * Structure GIF (très simple) :
 *   [6] Header : "GIF87a" ou "GIF89a"
 *   [2] Largeur (little-endian)
 *   [2] Hauteur (little-endian)
 *   [1] Packed byte (infos sur la palette)
 */
void ExifParser::_parseGifMetadata(const std::vector<unsigned char> &data) {
  std::cout << "  --- Métadonnées GIF ---" << std::endl;

  if (!_isOffsetSafe(data, 0, 13)) {
    std::cerr << "  Fichier GIF trop court." << std::endl;
    return;
  }

  /* Version : GIF87a ou GIF89a */
  std::string version;
  size_t i = 0;
  while (i < 6) {
    version += (char)data[i];
    i++;
  }
  std::cout << "  Version           : " << version << std::endl;

  /* Dimensions (little-endian) */
  uint16_t width = _read16(data, 6, false);
  uint16_t height = _read16(data, 8, false);
  std::cout << "  Dimensions        : " << width << " x " << height
            << std::endl;

  /* Palette globale : bit 7 du packed byte */
  uint8_t packed = data[10];
  bool hasGlobalPalette = (packed & 0x80) != 0;
  if (hasGlobalPalette) {
    int colorBits = (packed & 0x07) + 1;
    int numColors = 1;
    int k = 0;
    while (k < colorBits) {
      numColors = numColors * 2;
      k++;
    }
    std::cout << "  Couleurs          : " << numColors << std::endl;
  }
}

/* ========================================================================== */
/*                           PARSING BMP                                     */
/* ========================================================================== */

/*
 * _parseBmpMetadata() — Extrait les métadonnées d'un fichier BMP.
 *
 * Structure BMP :
 *   [14] File Header : "BM" + taille + réservé + offset données
 *   [40] DIB Header (BITMAPINFOHEADER) :
 *     [4] taille header  [4] largeur  [4] hauteur
 *     [2] plans          [2] bits/pixel
 *     [4] compression
 *   Tout est en little-endian.
 */
void ExifParser::_parseBmpMetadata(const std::vector<unsigned char> &data) {
  std::cout << "  --- Métadonnées BMP ---" << std::endl;

  /* Le DIB header commence à l'offset 14 */
  if (!_isOffsetSafe(data, 14, 40)) {
    std::cerr << "  Fichier BMP trop court." << std::endl;
    return;
  }

  /* Largeur et hauteur (signed, little-endian) */
  int32_t width = (int32_t)_read32(data, 18, false);
  int32_t height = (int32_t)_read32(data, 22, false);

  /* La hauteur peut être négative (image top-down) */
  if (height < 0)
    height = -height;

  std::cout << "  Dimensions        : " << width << " x " << height
            << std::endl;

  /* Bits par pixel */
  uint16_t bitsPerPixel = _read16(data, 28, false);
  std::cout << "  Bits/Pixel        : " << bitsPerPixel << std::endl;

  /* Compression */
  uint32_t compression = _read32(data, 30, false);
  std::string compStr = "Unknown";
  if (compression == 0)
    compStr = "None (BI_RGB)";
  else if (compression == 1)
    compStr = "RLE8";
  else if (compression == 2)
    compStr = "RLE4";
  else if (compression == 3)
    compStr = "Bitfields";
  std::cout << "  Compression       : " << compStr << std::endl;
}

/* ========================================================================== */
/*                          POINT D'ENTRÉE PUBLIC                            */
/* ========================================================================== */

/*
 * displayExifData() — Détecte le format du fichier et affiche ses métadonnées.
 *
 * Formats gérés :
 *   JPEG → Parsing EXIF (IFD0 + Exif Sub-IFD)
 *   PNG  → IHDR (dimensions) + chunks tEXt
 *   GIF  → Header (version, dimensions, palette)
 *   BMP  → DIB Header (dimensions, bits/pixel, compression)
 */
void ExifParser::displayExifData(const std::string &filename) {
  std::vector<unsigned char> data = _readFile(filename);
  if (data.empty())
    return;

  /* Détection du format via les magic numbers */

  /* JPEG : FF D8 FF */
  if (data.size() >= 3 && data[0] == 0xFF && data[1] == 0xD8 &&
      data[2] == 0xFF) {
    int tiffStart = _findExifOffset(data);
    if (tiffStart < 0) {
      std::cout << "  Aucune donnée EXIF trouvée." << std::endl;
      return;
    }

    bool bigEndian = _isBigEndian(data, tiffStart);
    uint16_t magic = _read16(data, tiffStart + 2, bigEndian);
    if (magic != 0x002A) {
      std::cerr << "  TIFF magic invalide." << std::endl;
      return;
    }

    std::cout << "  --- Données EXIF ---" << std::endl;
    uint32_t ifd0Offset = _read32(data, tiffStart + 4, bigEndian);
    _parseIFD(data, tiffStart, ifd0Offset, bigEndian, false);
    return;
  }

  /* PNG : 89 50 4E 47 */
  if (data.size() >= 4 && data[0] == 0x89 && data[1] == 0x50 &&
      data[2] == 0x4E && data[3] == 0x47) {
    _parsePngMetadata(data);
    return;
  }

  /* GIF : "GIF8" */
  if (data.size() >= 4 && data[0] == 0x47 && data[1] == 0x49 &&
      data[2] == 0x46 && data[3] == 0x38) {
    _parseGifMetadata(data);
    return;
  }

  /* BMP : "BM" */
  if (data.size() >= 2 && data[0] == 0x42 && data[1] == 0x4D) {
    _parseBmpMetadata(data);
    return;
  }

  std::cout << "  Format non supporté pour le parsing." << std::endl;
}
