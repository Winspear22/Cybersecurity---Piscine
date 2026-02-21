/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UseCurl.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 21:59:44 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 22:09:29 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UseCurl.hpp"

UseCurl::UseCurl(void)
{
	std::cout << "Constructeur de UseCurl utilisé." << std::endl;
}

UseCurl::UseCurl(const UseCurl &src)
{
	std::cout << "Constructeur de recopie UseCurl utilisé." << std::endl;
	*this = src;
}

const UseCurl &UseCurl::operator=(const UseCurl &src)
{
	std::cout << "Operatot= de UseCurl utilisé." << std::endl;
    if (this == &src)
        return *this;
    return *this;
}

UseCurl::~UseCurl(void)
{
	std::cout << "Destructeur de UseCurl utilisé." << std::endl;
}

size_t	UseCurl::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t			realsize;
	std::string		*buffer;
	
	realsize = size * nmemb;
	buffer = static_cast<std::string *>(userp);
	buffer->append(static_cast<char *>(contents), realsize);
	return realsize;
}
