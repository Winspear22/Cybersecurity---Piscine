/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:22:34 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 14:26:29 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "includes.hpp"
#include "UrlHelper.hpp"

class Parser
{
	public:
		Parser();
		Parser(const Parser &src);
		Parser &operator=(const Parser &src);
		~Parser();

        std::set<std::string> extract_images(const std::string& html, const std::string& start_url, UrlHelper& urlHelper);
        
        // Retourne un VECTOR de liens (pour la récursion)
        std::vector<std::string> extract_links(const std::string& html, const std::string& start_url, UrlHelper& urlHelper);
};

#endif