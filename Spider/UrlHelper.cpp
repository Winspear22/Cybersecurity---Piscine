/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UrlHelper.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:52:20 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 14:29:47 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UrlHelper.hpp"

UrlHelper::UrlHelper() {}
UrlHelper::~UrlHelper() {}
UrlHelper::UrlHelper(const UrlHelper &src) { *this = src; }
UrlHelper &UrlHelper::operator=(const UrlHelper &src) 
{ 
	if (this != &src) {} 
	return *this; 
}

std::string UrlHelper::_resolve_url(const std::string& base_url, const std::string& link_url)
{
	if (link_url.find("http") == 0)
		return link_url;
	else if (link_url.find("//") == 0)
		return "http:" + link_url;
	
	// 1. Déclarations
	std::string domain;
	std::string path;
	
	// 2. Extraction du DOMAINE
	size_t protocolPos = base_url.find("://");
	// Si on ne trouve pas le protocole, on suppose que c'est du http implicite ou qu'on ne peut rien faire
	if (protocolPos == std::string::npos) 
		return link_url; // Cas fallback

	size_t domainEnds = base_url.find("/", protocolPos + 3);
	if (domainEnds != std::string::npos)
		domain = base_url.substr(0, domainEnds);
	else
		domain = base_url;
	
	// 3. Extraction du PATH
	size_t lastSlash = base_url.find_last_of("/");
	if (lastSlash != std::string::npos && lastSlash > protocolPos + 2)
        path = base_url.substr(0, lastSlash + 1);
    else
        path = base_url + "/";

	// 4. Assemblage
    if (link_url.find("/") == 0)
        return domain + link_url;
    else
        return path + link_url;
}

bool UrlHelper::_is_valid_extension(const std::string& url)
{
	size_t pos;
	size_t i;
	std::vector<std::string> validExtensions = {".jpg", ".jpeg", ".png", ".gif", ".bmp", ".webp"};

    std::string path = url;

	// 2. NETTOYAGE : On enlève tout ce qui est après le '?' (les paramètres)
	size_t query_pos = path.find('?');
	if (query_pos != std::string::npos)
	{
		path = path.substr(0, query_pos);
	}

	// 3. Extraction de l'extension
	pos = path.find_last_of('.');
	if (pos == std::string::npos)
		return (FAILURE);

	std::string extension = path.substr(pos);	
	i = 0;
	while (i < extension.length())
	{
		extension[i] = std::tolower(extension[i]);
		i++;
	}

	i = 0;
	while (i < validExtensions.size())
	{
		if (extension == validExtensions[i])
			return (SUCCESS);
		i++;
	}
	return (FAILURE);
}