/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scorpion.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:50:17 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 14:52:27 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scorpion.hpp"

Scorpion::Scorpion()
{
	std::cout << PURPLE << "Scorpion constructor called" << RESET << std::endl;
}

Scorpion::~Scorpion()
{
	std::cout << PURPLE << "Scorpion destructor called" << RESET << std::endl;
}

Scorpion::Scorpion(const Scorpion &src)
{
	std::cout << PURPLE << "Scorpion copy constructor called" << RESET << std::endl;
}

Scorpion &Scorpion::operator=(const Scorpion &src)
{
	std::cout << PURPLE << "Scorpion copy assignment operator called" << RESET << std::endl;
}
