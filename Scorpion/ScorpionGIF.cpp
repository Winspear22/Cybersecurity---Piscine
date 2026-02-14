/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionGIF.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:38:51 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 15:16:35 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScorpionGIF.hpp"

ScorpionGIF::ScorpionGIF()
{
    std::cout << MAGENTA << "ScorpionGIF constructor called" << RESET
              << std::endl;
}

ScorpionGIF::~ScorpionGIF()
{
    std::cout << MAGENTA << "ScorpionGIF destructor called" << RESET
              << std::endl;
}

ScorpionGIF::ScorpionGIF(const ScorpionGIF &src) : Scorpion(src)
{
    std::cout << MAGENTA << "ScorpionGIF copy constructor called" << RESET
              << std::endl;
    *this = src;
}

ScorpionGIF &ScorpionGIF::operator=(const ScorpionGIF &src)
{
    std::cout << MAGENTA << "ScorpionGIF copy assignment operator called"
              << RESET << std::endl;
    (void)src;
    return *this;
}

void ScorpionGIF::parseSpecific(std::ifstream &file)
{
    // ============================================
    // ÉTAPE 1 : Sauter le header GIF ("GIF87a" ou "GIF89a" = 6 octets)
    // ============================================
    file.seekg(6, std::ios::beg);

    // ============================================
    // ÉTAPE 2 : Lire le Logical Screen Descriptor
    //           Width (2 octets Little Endian) + Height (2 octets Little
    //           Endian)
    // ============================================
    unsigned char wBytes[2], hBytes[2];
    file.read((char *)wBytes, 2);
    file.read((char *)hBytes, 2);

    unsigned short width = wBytes[0] + (wBytes[1] * 256);
    unsigned short height = hBytes[0] + (hBytes[1] * 256);

    std::cout << GREEN << "  [GIF]    Dimensions : " << width << " x " << height
              << RESET << std::endl;
    std::cout << YELLOW << "  [EXIF]   GIF does not support EXIF metadata"
              << RESET << std::endl;
}
