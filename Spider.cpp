/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spider.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:12:40 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 20:26:25 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Spider.hpp"

Spider::Spider(void)
{
	std::cout << "Constructeur de Spider utilisé." << std::endl;
}

Spider::Spider(const Spider &src)
{
	(void)src;
	std::cout << "Constructeur de recopie Spider utilisé." << std::endl;
}

const Spider &Spider::operator=(const Spider &src)
{
	std::cout << "Operatot= de Spider utilisé." << std::endl;
	(void)src;
	return *this;
}

Spider::~Spider(void)
{
	std::cout << "Destructeur de Spider utilisé." << std::endl;
}