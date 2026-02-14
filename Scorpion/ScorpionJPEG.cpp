/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionJPEG.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:02:36 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 15:15:46 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScorpionJPEG.hpp"

ScorpionJPEG::ScorpionJPEG()
{
    std::cout << MAGENTA << "ScorpionJPEG constructor called" << RESET
              << std::endl;
}

ScorpionJPEG::~ScorpionJPEG()
{
    std::cout << MAGENTA << "ScorpionJPEG destructor called" << RESET
              << std::endl;
}

ScorpionJPEG::ScorpionJPEG(const ScorpionJPEG &src) : Scorpion(src)
{
    std::cout << MAGENTA << "ScorpionJPEG copy constructor called" << RESET
              << std::endl;
    *this = src;
}

ScorpionJPEG &ScorpionJPEG::operator=(const ScorpionJPEG &src)
{
    std::cout << MAGENTA << "ScorpionJPEG copy assignment operator called"
              << RESET << std::endl;
    (void)src;
    return *this;
}

void ScorpionJPEG::parseSpecific(std::ifstream &file)
{
    /*
          METHODE 1 : BRUTE FORCE (Active)
          Scanner chaque octet jusqu'à trouver FF E1.
    */

    file.seekg(2, std::ios::beg);
    char byte;
    while (file.get(byte))
    {
        if ((unsigned char)byte == 0xFF)
        {
            if (file.get(byte))
            {
                if ((unsigned char)byte == 0xE1)
                {
                    // On a trouvé le marker FF E1 !
                    // Les 2 octets suivants sont la taille (on les ignore pour
                    // l'instant)
                    file.seekg(2, std::ios::cur);

                    char header[6];
                    file.read(header, 6);
                    if (std::string(header, 4) == "Exif")
                    {
                        std::cout << BOLD_CYAN
                                  << "  [EXIF]   Found APP1 segment" << RESET
                                  << std::endl;
                        this->_parseTiff(file);
                        return;
                    }
                }
                else if ((unsigned char)byte == 0xDA)
                    // SOS : Start Of Scan = Début de l'image compressée
                    break;
            }
        }
    }
    std::cout << YELLOW << "  [EXIF]   No metadata found" << RESET << std::endl;
}
