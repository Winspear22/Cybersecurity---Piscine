/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 15:53:12 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 15:59:44 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scorpion.hpp"

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
		Scorpion scorpion;
		while (++i <= argc)
		{
			std::cout << "\n--- Analyzing: " << argv[i] << " ---" << std::endl;
			scorpion.FileIdentification(argv[i]);
		}
	}
	return 0;
}