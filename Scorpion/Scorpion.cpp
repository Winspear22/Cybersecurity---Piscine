/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scorpion.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:50:17 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 15:48:38 by adnen            ###   ########.fr       */
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

void 	Scorpion::FileIdentification(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << BOLD_RED << "Error: could not open " << filename << RESET << std::endl;
		return ;
	}
	unsigned char headOfFile[12];
	file.read((char*)headOfFile, 12);
	if (file.gcount() < 2)
	{
    	std::cerr << RED << "File too small to be an image" << RESET << std::endl;
    	return;
	}
	
	if (headOfFile[0] == 0xFF && headOfFile[1] == 0xD8)
		std::cout << GREEN << "It's a jpeg/jpg file !" << RESET << std::endl;
	else if (headOfFile[0] == 0x89 && headOfFile[1] == 0x50)
		std::cout << BLUE << "It's a png file !" << RESET << std::endl;
	else if (headOfFile[0] == 0x47 && headOfFile[1] == 0x49)
		std::cout << YELLOW << "It's a GIF file !" << RESET << std::endl;
	else if (headOfFile[0] == 0x42 && headOfFile[1] == 0x4D)
		std::cout << MAGENTA << "It's a bmp file !" << RESET << std::endl;
	else if (headOfFile[0] == 0x52 && headOfFile[1] == 0x49)
	{
		if (headOfFile[8] == 'W' && headOfFile[9] == 'E' && headOfFile[10] == 'B' && headOfFile[11] == 'P')
			std::cout << CYAN << "It's a webp file !" << RESET << std::endl;
		else
			std::cout << RED << "It's not a valid image file (.wev or .avi) !" << RESET << std::endl;
	}
	else
		std::cout << RED << "It's not a valid image file !" << RESET << std::endl;
	
}

