/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 15:53:12 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 15:35:49 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScorpionBMP.hpp"
#include "ScorpionGIF.hpp"
#include "ScorpionJPEG.hpp"
#include "ScorpionPNG.hpp"
#include "ScorpionWEBP.hpp"

int main(int argc, char **argv)
{
    int i;

    i = 0;
    if (argc < 2)
    {
        std::cout << BOLD_CYAN << "Usage: ./scorpion FILE1 [FILE2 ...]" << RESET << std::endl;
        return 1;
    }
    else
    {
        while (++i < argc)
        {
            std::cout << BOLD_CYAN << "\n--- Analyzing: " << argv[i] << " ---" << RESET << std::endl;
            Scorpion::FileIdentification(argv[i]);
        }
    }
    return 0;
}