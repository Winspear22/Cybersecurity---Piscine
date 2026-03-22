/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 15:53:12 by adnen             #+#    #+#             */
/*   Updated: 2026/03/22 07:16:15 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MetadataEditor.hpp"
#include "ScorpionBMP.hpp"
#include "ScorpionGIF.hpp"
#include "ScorpionJPEG.hpp"
#include "ScorpionPNG.hpp"
#include "ScorpionWEBP.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << BOLD_CYAN << "Usage: ./scorpion_bonus [--delete FILE | --modify FILE TAG VALUE | FILE1 FILE2 ...]"
                  << RESET << std::endl;
        return 1;
    }

    std::string firstArg = argv[1];

    // --- Option --delete ---
    if (firstArg == "--delete")
    {
        if (argc < 3)
        {
            std::cerr << BOLD_RED << "Usage: ./scorpion_bonus --delete FILE"
                      << RESET << std::endl;
            return 1;
        }
        if (MetadataEditor::deleteMetadata(argv[2]))
            return 0;
        return 1;
    }

    // --- Mode normal (analyse) ---
    int i = 0;
    while (++i < argc)
    {
        std::cout << BOLD_CYAN << "\n--- Analyzing: " << argv[i] << " ---"
                  << RESET << std::endl;
        Scorpion::FileIdentification(argv[i]);
    }
    return 0;
}
