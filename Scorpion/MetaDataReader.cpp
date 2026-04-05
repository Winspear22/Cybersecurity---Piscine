/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetaDataReader.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 23:52:28 by adnen             #+#    #+#             */
/*   Updated: 2026/04/05 16:11:53 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MetaDataReader.hpp"

MetaDataReader::MetaDataReader()
{
	std::cout << "MetaDataReader constructor called." << std::endl;
}

MetaDataReader::~MetaDataReader()
{
	std::cout << "MetaDataReader destructor called." << std::endl;
}

const MetaDataReader &MetaDataReader::operator=(const MetaDataReader &other)
{
	if (this != &other) {}
	std::cout << "MetaDataReader copy assignment operator called." << std::endl;
	return *this;
}

MetaDataReader::MetaDataReader(const MetaDataReader &other)
{
	*this = other;
	std::cout << "MetaDataReader copy constructor called." << std::endl;
}

/*
 * displayMetadata() — Affiche les infos filesystem d'un fichier image
 */
void MetaDataReader::displayMetadata(const std::string &filename)
{
	std::filesystem::path path(filename);

	std::cout << "══════════════════════════════════════" << std::endl;
	std::cout << "---------- COMMON METADATA -----------" << std::endl;
	std::cout << "══════════════════════════════════════" << std::endl;
	std::cout << "  File name            : " << path.filename().string() << std::endl;
	

	// On normalise l'extension en minuscules
	std::string ext = path.extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) 
	{
		return std::tolower(c); 
	});
	std::string type = "Unknown";
	if (ext == ".jpg" || ext == ".jpeg")
    	type = "JPEG";
	else if (ext == ".png")
    	type = "PNG";
	else if (ext == ".gif")
    	type = "GIF";
	else if (ext == ".bmp")
    	type = "BMP";

	std::cout << "  Type                 : " << type << std::endl;
	std::cout << "  Taille               : " << _formatSize(std::filesystem::file_size(path)) << std::endl;
	std::cout << "  Créé/Changé(statut)  : " << _formatCreationDate(filename) << std::endl;
	std::cout << "  Modifié(contenu)     : " << _formatDate(filename) << std::endl;
	std::cout << "  Permissions          : " << _formatPermissions(std::filesystem::status(path).permissions()) << std::endl;
	std::cout << "══════════════════════════════════════" << std::endl;
	std::cout << std::endl;
	std::cout << "══════════════════════════════════════" << std::endl;
	std::cout << "---------- SPECIFIC METADATA ---------" << std::endl;
	std::cout << "══════════════════════════════════════" << std::endl;
}

/*
 * _formatSize() — Convertit les octets en format lisible (octets, Ko, Mo)
 */
std::string MetaDataReader::_formatSize(uintmax_t bytes)
{
	std::ostringstream oss; // création d'un flux de sortie comme std::cout mais en mémoire
	//oss << std::fixed << std::setprecision(2); // on fixe la précision à 2 chiffres après la virgule

	if (bytes >= 1048576) // 1 Mo = 1024 * 1024 octets donc, si c'est plus grand que 1 Mo, on affiche en Mo
		oss << static_cast<double>(bytes) / 1048576.0 << " Mo"; // On cast en double pour afficher les décimales et non pas un nombre entier ex : int resultat = 1500 / 1024 = 1 au lieu de 1.4648 si on met un double
	else if (bytes >= 1024) // 1 Ko = 1024 octets donc, si c'est plus grand que 1 Ko, on affiche en Ko
		oss << static_cast<double>(bytes) / 1024.0 << " Ko";
	else // sinon on affiche en octets
		oss << bytes << " octets";
	return oss.str();
}

/*
 * _formatDate() — Cherche à trouver la date de modification du fichier
 */
std::string MetaDataReader::_formatDate(const std::string &filename)
{
	struct stat fileStat; // structure qui contient les informations d'un fichier (ex : date de création, date de modification, taille, permissions, etc.)

	if (stat(filename.c_str(), &fileStat) != 0)
		return ("Date inconnue");

	std::time_t modTime = fileStat.st_mtime; // Change uniquement lorsque le contenu du fichier est modifié (ex : pixels changés)
	char buf[64];
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&modTime));
	return (std::string(buf));
}

/*
 * _formatCreationDate() — Cherche à trouver la date de création du fichier
 */
std::string MetaDataReader::_formatCreationDate(const std::string &filename)
{
	struct stat fileStat;

	if (stat(filename.c_str(), &fileStat) != 0)
		return ("Date inconnue");

	std::time_t createTime = fileStat.st_ctime; // Change uniquement lorsque le fichier change de statut (ex: changer le nom ou les permissions)
	char buf[64];
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&createTime)); // Transformer le format de la date en string lisible Y/M/D H:M:S
	return (std::string(buf));
}

/*
 * _formatPermissions() — Construit une string "rwxr-xr--" style ls -l
 */
std::string MetaDataReader::_formatPermissions(std::filesystem::perms p)
{
	//std::cout << "TEST TEST TESt === " << static_cast<unsigned int>(p) << std::endl; --> AFFICHERA un nombre en base 10, il faut le convertir en octal pour le calquer sur chmod
	std::string result;
	using perms = std::filesystem::perms; // using est l'équivalent de typedef en C, donc on renomme juste le bail pour éviter de faire long

	if ((p & perms::owner_read) != perms::none) // '&' est "Masque binaire" et permet de vérifier si un bit est activé ou éteint
		result += "r";							// Si le bit est activé, on ajoute 'r' à la string, sinon on ajoute '-'
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
