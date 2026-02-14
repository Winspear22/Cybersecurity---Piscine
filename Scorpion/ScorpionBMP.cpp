/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionBMP.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:35:59 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 15:16:58 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScorpionBMP.hpp"

ScorpionBMP::ScorpionBMP()
{
    std::cout << MAGENTA << "ScorpionBMP constructor called" << RESET
              << std::endl;
}

ScorpionBMP::~ScorpionBMP()
{
    std::cout << MAGENTA << "ScorpionBMP destructor called" << RESET
              << std::endl;
}

ScorpionBMP::ScorpionBMP(const ScorpionBMP &src) : Scorpion(src)
{
    std::cout << MAGENTA << "ScorpionBMP copy constructor called" << RESET
              << std::endl;
    *this = src;
}

ScorpionBMP &ScorpionBMP::operator=(const ScorpionBMP &src)
{
    std::cout << MAGENTA << "ScorpionBMP copy assignment operator called"
              << RESET << std::endl;
    (void)src;
    return *this;
}

void ScorpionBMP::parseSpecific(std::ifstream &file)
{
    // ============================================
    // ÉTAPE 1 : Sauter le BMP File Header (14 octets)
    //           puis lire le DIB Header (BITMAPINFOHEADER)
    //           Offset 18 = Width (4 octets LE), Offset 22 = Height (4 octets
    //           LE)
    // ============================================
    file.seekg(18, std::ios::beg);

    unsigned char wBytes[4], hBytes[4];
    file.read((char *)wBytes, 4);
    file.read((char *)hBytes, 4);

    // Little Endian
    long width = wBytes[0] + (wBytes[1] * 256) + (wBytes[2] * 256 * 256) +
                 (wBytes[3] * 256 * 256 * 256);
    long height = hBytes[0] + (hBytes[1] * 256) + (hBytes[2] * 256 * 256) +
                  (hBytes[3] * 256 * 256 * 256);

    // Height peut être négatif (top-down bitmap), on prend la valeur absolue
    if (height < 0) height = -height;

    std::cout << GREEN << "  [BMP]    Dimensions : " << width << " x " << height
              << RESET << std::endl;
    std::cout << YELLOW << "  [EXIF]   BMP does not support EXIF metadata"
              << RESET << std::endl;
}
