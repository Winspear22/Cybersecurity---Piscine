/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetaDataReader.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 23:52:28 by adnen             #+#    #+#             */
/*   Updated: 2026/03/03 00:02:50 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MetaDataReader.hpp"

MetaDataReader::MetaDataReader() {
  std::cout << "MetaDataReader constructor called." << std::endl;
}

MetaDataReader::~MetaDataReader() {
  std::cout << "MetaDataReader destructor called." << std::endl;
}

const MetaDataReader &MetaDataReader::operator=(const MetaDataReader &other) {
  if (this != &other) {
  }
  std::cout << "MetaDataReader copy assignment operator called." << std::endl;
  return *this;
}

MetaDataReader::MetaDataReader(const MetaDataReader &other) {
  *this = other;
  std::cout << "MetaDataReader copy constructor called." << std::endl;
}

/*
 * displayMetadata() — Affiche les infos filesystem d'un fichier image
 */
void MetaDataReader::displayMetadata(const std::string &filename) {
  std::filesystem::path p(filename);

  std::cout << "══════════════════════════════════════" << std::endl;
  std::cout << "📄 " << p.filename().string() << std::endl;
  std::cout << "══════════════════════════════════════" << std::endl;

  // Extension normalisée → type
  std::string ext = p.extension().string();
  std::transform(ext.begin(), ext.end(), ext.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  std::string type = "Unknown";
  if (ext == ".jpg" || ext == ".jpeg")
    type = "JPEG";
  else if (ext == ".png")
    type = "PNG";
  else if (ext == ".gif")
    type = "GIF";
  else if (ext == ".bmp")
    type = "BMP";

  std::cout << "  Type         : " << type << std::endl;
  std::cout << "  Taille       : " << _formatSize(std::filesystem::file_size(p))
            << std::endl;
  std::cout << "  Modifié      : " << _formatDate(filename) << std::endl;
  std::cout << "  Permissions  : "
            << _formatPermissions(std::filesystem::status(p).permissions())
            << std::endl;
  std::cout << "══════════════════════════════════════" << std::endl;
}

/*
 * _formatSize() — Convertit les octets en format lisible (octets, Ko, Mo)
 */
std::string MetaDataReader::_formatSize(uintmax_t bytes) {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(2);

  if (bytes >= 1048576)
    oss << (double)bytes / 1048576.0 << " Mo";
  else if (bytes >= 1024)
    oss << (double)bytes / 1024.0 << " Ko";
  else
    oss << bytes << " octets";
  return oss.str();
}

/*
 * _formatDate() — Utilise stat() pour obtenir la date de modification
 *                 C'est beaucoup plus simple que la conversion chrono C++17
 */
std::string MetaDataReader::_formatDate(const std::string &filename) {
  struct stat fileStat;

  if (stat(filename.c_str(), &fileStat) != 0)
    return "Date inconnue";

  std::time_t modTime = fileStat.st_mtime;
  char buf[64];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S",
                std::localtime(&modTime));
  return std::string(buf);
}

/*
 * _formatPermissions() — Construit une string "rwxr-xr--" style ls -l
 */
std::string MetaDataReader::_formatPermissions(std::filesystem::perms p)
{
  std::string result;
  using perms = std::filesystem::perms;

  if ((p & perms::owner_read) != perms::none)
    result += "r";
  else
    result += "-";
  if ((p & perms::owner_write) != perms::none)
    result += "w";
  else
    result += "-";
  if ((p & perms::owner_exec) != perms::none)
    result += "x";
  else
    result += "-";
  if ((p & perms::group_read) != perms::none)
    result += "r";
  else
    result += "-";
  if ((p & perms::group_write) != perms::none)
    result += "w";
  else
    result += "-";
  if ((p & perms::group_exec) != perms::none)
    result += "x";
  else
    result += "-";
  if ((p & perms::others_read) != perms::none)
    result += "r";
  else
    result += "-";
  if ((p & perms::others_write) != perms::none)
    result += "w";
  else
    result += "-";
  if ((p & perms::others_exec) != perms::none)
    result += "x";
  else
    result += "-";
  return result;
}
