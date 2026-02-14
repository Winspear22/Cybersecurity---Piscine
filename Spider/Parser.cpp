/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:22:27 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 14:33:15 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

#include <iostream>
#include <set>
#include <vector>

Parser::Parser()
{
}
Parser::~Parser()
{
}
Parser::Parser(const Parser &src)
{
    *this = src;
}
Parser &Parser::operator=(const Parser &src)
{
    if (this != &src)
    {
    }
    return *this;
}

std::set<std::string> Parser::extract_images(const std::string &html,
                                             const std::string &start_url,
                                             UrlHelper &urlHelper)
{
    std::set<std::string> found_images;
    std::regex regex(R"(<img[^>]*src="([^"]*))");
    std::sregex_iterator it(html.begin(), html.end(), regex);
    std::sregex_iterator end;

    while (it != end)
    {
        std::smatch match = *it;
        if (match.size() > 1)
        {
            std::string raw_url = match[1];
            // On nettoie l'URL
            std::string clean_url = urlHelper._resolve_url(start_url, raw_url);

            // Si le résultat est valide (commence par http), on l'ajoute
            if (clean_url.find("http") == 0)
            {
                if (urlHelper._is_valid_extension(clean_url))
                    found_images.insert(clean_url);
            }
        }
        ++it;
    }
    return found_images;  // On retourne la liste
}

std::vector<std::string> Parser::extract_links(const std::string &html_content,
                                               const std::string &start_url,
                                               UrlHelper &urlHelper)
{
    std::vector<std::string> found_links;
    std::regex regex(R"(<a[^>]*href=["']([^"']+)["'])");
    std::sregex_iterator it(html_content.begin(), html_content.end(), regex);
    std::sregex_iterator end;

    while (it != end)
    {
        std::smatch match = *it;
        if (match.size() > 1)
        {
            std::string raw_url = match[1];
            std::string clean_url = urlHelper._resolve_url(start_url, raw_url);
            found_links.push_back(clean_url);
        }
        ++it;
    }
    return found_links;
}