/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExifParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:03:42 by adnen             #+#    #+#             */
/*   Updated: 2026/03/03 00:04:16 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExifParser.hpp"

ExifParser::ExifParser()
{
	std::cout << "ExifParser constructor called." << std::endl;
}

ExifParser::~ExifParser()
{
	std::cout << "ExifParser destructor called." << std::endl;
}

const ExifParser &ExifParser::operator=(const ExifParser &other)
{
	if (this != &other)
	{
	}
	std::cout << "ExifParser copy assignment operator called." << std::endl;
	return *this;
}

ExifParser::ExifParser(const ExifParser &other)
{
	*this = other;
	std::cout << "ExifParser copy constructor called." << std::endl;
}
