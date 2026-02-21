/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spider.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:12:40 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 21:04:32 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Spider.hpp"

Spider::Spider(void)
{
	std::cout << "Constructeur de Spider utilisé." << std::endl;
	this->_depthNumber = 0;
	this->_isRecursive = false;
	this->_pathOfDownload = "./data/";
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

bool			Spider::getIsRecursive(void)
{
	return this->_isRecursive;
}

void			Spider::setIsRecursive(bool isRecursive)
{
	this->_isRecursive = isRecursive;
}

int				Spider::getDepthNumber(void)
{
	return this->_depthNumber;
}

void			Spider::setDepthNumber(int nb)
{
	this->_depthNumber = nb;
}

std::string		Spider::getPathOfDownload(void)
{
	return this->_pathOfDownload;
}

void			Spider::setPathOfDownload(std::string path)
{
	this->_pathOfDownload = path;
}

std::string		Spider::getUrl(void)
{
	return this->_url;
}

void			Spider::setUrl(std::string url)
{
	this->_url = url;
}