/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UrlParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 17:28:08 by adnen             #+#    #+#             */
/*   Updated: 2026/02/22 17:28:35 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UrlParser.hpp"

UrlParser::UrlParser(void)
{
	std::cout << "Constructeur de UrlParser utilisé." << std::endl;
}

UrlParser::UrlParser(const UrlParser &src)
{
	std::cout << "Constructeur de recopie UrlParser utilisé." << std::endl;
	*this = src;
}

const UrlParser &UrlParser::operator=(const UrlParser &src)
{
	std::cout << "Operator= de UrlParser utilisé." << std::endl;
	if (this == &src)
		return *this;
	return *this;
}

UrlParser::~UrlParser(void)
{
	std::cout << "Destructeur de UrlParser utilisé." << std::endl;
}