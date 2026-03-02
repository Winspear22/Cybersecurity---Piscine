/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Crawler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 20:49:54 by adnen             #+#    #+#             */
/*   Updated: 2026/03/02 20:50:20 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Crawler.hpp"

Crawler::Crawler(void)
{
	std::cout << "Constructeur de Crawler utilisé." << std::endl;
}

Crawler::Crawler(const Crawler &src)
{
	std::cout << "Constructeur de recopie Crawler utilisé." << std::endl;
	*this = src;
}

const Crawler &Crawler::operator=(const Crawler &src)
{
	std::cout << "Operator= de Crawler utilisé." << std::endl;
	if (this == &src)
		return *this;
	return *this;
}

Crawler::~Crawler(void)
{
	std::cout << "Destructeur de Crawler utilisé." << std::endl;
}
