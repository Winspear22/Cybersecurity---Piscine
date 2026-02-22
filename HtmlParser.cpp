/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 15:46:37 by adnen             #+#    #+#             */
/*   Updated: 2026/02/22 16:21:28 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HtmlParser.hpp"

HtmlParser::HtmlParser(void)
{
    std::cout << "Constructeur de HtmlParser utilisé." << std::endl;
}

HtmlParser::HtmlParser(const HtmlParser &src)
{
    std::cout << "Constructeur de recopie HtmlParser utilisé." << std::endl;
	*this = src;
}

const HtmlParser &HtmlParser::operator=(const HtmlParser &src)
{
	std::cout << "Operator= de HtmlParser utilisé." << std::endl;
	if (this == &src)
		return *this;
	return *this;
}

HtmlParser::~HtmlParser(void)
{
	std::cout << "Destructeur de HtmlParser utilisé." << std::endl;
}

/* 
 * Regex breakdown :
 * <img       : Cherche le début de la balise (insensible à la casse via icase)
 * [^>]+      : [^>] veut dire "pas >", le + veut dire "au moins un". Saute les autres attributs.
 * src        : Cherche le mot "src"
 * \s*=\s*    : \s* (espaces facultatifs) suivis de = puis encore \s* (espaces facultatifs)
 * ["']       : Cherche soit un guillemet double " soit un simple '
 * ([^"']+)   : (...) = Capture ce qu'il y a dedans. [^"']+ = au moins un caractère qui n'est ni " ni '
 * ["']       : Cherche le guillemet de fermeture correspond au début
 */
std::vector<std::string> HtmlParser::extractImagesFromHtml(const std::string &html)
{
	
	std::vector<std::string> urls;
	std::regex imgRegex(R"(<img[^>]+src\s*=\s*["']([^"']+)["'])", std::regex::icase);

	std::sregex_iterator it;
	std::sregex_iterator begin(html.begin(), html.end(), imgRegex);
	std::sregex_iterator end;
	
	it = begin;
	while (it != end)
	{
		std::smatch match = *it;
		std::string imgUrl = match[1].str();
		urls.push_back(imgUrl);
		it++;
	}
	return urls;
}