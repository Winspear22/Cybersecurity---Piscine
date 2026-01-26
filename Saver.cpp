/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Saver.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:56:19 by adnen             #+#    #+#             */
/*   Updated: 2026/01/26 13:36:21 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Saver.hpp"
Saver::Saver()
{
	std::cout << BOLD_BLUE << "Saver constructor called" << RESET << std::endl;
}

Saver::Saver(const Saver &src)
{
	std::cout << BOLD_GREEN << "Saver copy constructor called" << RESET << std::endl;
	*this = src;
}

Saver &Saver::operator=(const Saver &src)
{
	std::cout << BOLD_GREEN << "Saver copy assignment operator called" << RESET << std::endl;
	if (this != &src) {}
	return *this;
}

Saver::~Saver()
{
	std::cout << BOLD_BLUE << "Saver destructor called" << RESET << std::endl;
}

void Saver::save_file(const std::string& data, const std::string& url)
{
	std::string fileName = this->_extract_filename(url);
	// On utilise le "pipe" | pour additionner les options
	std::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary);	
	if (!file.is_open())
	{
		std::cerr << BOLD_RED << "Error: could not open file " << fileName << RESET << std::endl;
		return ;
	}
	file.write(data.c_str(), data.size());
	file.close();
}

std::string Saver::_extract_filename(const std::string& url)
{
	std::string slash = "/";
	size_t last_slash_pos = url.find_last_of(slash);
	if (last_slash_pos == std::string::npos)
		return "unknown.bin";
	std::string filename = url.substr(last_slash_pos + 1);
	std::cout << filename << std::endl;
	return filename;
}