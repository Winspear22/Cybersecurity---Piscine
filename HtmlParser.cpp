/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 15:46:37 by adnen             #+#    #+#             */
/*   Updated: 2026/02/22 15:47:22 by adnen            ###   ########.fr       */
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