/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImageDownloader.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 17:49:31 by adnen             #+#    #+#             */
/*   Updated: 2026/02/26 17:50:20 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ImageDownloader.hpp"

ImageDownloader::ImageDownloader(void)
{
	std::cout << "Constructeur de ImageDownloader utilisé." << std::endl;
}

ImageDownloader::ImageDownloader(const ImageDownloader &src)
{
	std::cout << "Constructeur de recopie ImageDownloader utilisé." << std::endl;
	*this = src;
}

const ImageDownloader &ImageDownloader::operator=(const ImageDownloader &src)
{
	std::cout << "Operator= de ImageDownloader utilisé." << std::endl;
	if (this == &src)
		return *this;
	this->_url = src._url;
	return *this;
}

ImageDownloader::~ImageDownloader(void)
{
	std::cout << "Destructeur de ImageDownloader utilisé." << std::endl;
}
