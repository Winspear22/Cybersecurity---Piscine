/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UrlUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 23:03:02 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 23:27:04 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UrlUtils.hpp"
#include "includes.hpp"

UrlUtils::UrlUtils(void)
{
	std::cout << "Constructeur de UrlUtils utilisé." << std::endl;
}

UrlUtils::UrlUtils(const UrlUtils &src)
{
	std::cout << "Constructeur de recopie UrlUtils utilisé." << std::endl;
	*this = src;
}

const UrlUtils &UrlUtils::operator=(const UrlUtils &src)
{
	std::cout << "Operator= de UrlUtils utilisé." << std::endl;
	if (this == &src)
		return *this;
	return *this;
}

UrlUtils::~UrlUtils(void)
{
	std::cout << "Destructeur de UrlUtils utilisé." << std::endl;
}

bool UrlUtils::parseArguments(int argc, char **argv, Spider &spider)
{
	int opt;
	
	while ((opt = getopt(argc, argv, "rl:p:")) != -1)
	{
		switch (opt)
		{
			case 'r':
				spider.setIsRecursive(true);
				break;
			case 'l':
				try 
				{
					spider.setDepthNumber(std::stoi(optarg));
				}
				catch (const std::invalid_argument &e)
				{
					std::cerr << "Error: Depth '-l' must be a valid number (e.g. -l 5)." << std::endl;
					return (FAILURE);
				} 
				catch (const std::out_of_range &e)
				{
					std::cerr << "Error: Depth number is too large." << std::endl;
					return (FAILURE);
				}
				break;
			case 'p':
				spider.setPathOfDownload(optarg);
				break;
			case '?':
				std::cerr << "Error, wrong set of options." << std::endl;
				return FAILURE;
		}
	}
	if (optind < argc)
		spider.setUrl(argv[optind]);
	else
	{
		std::cerr << "Error, Missing URL." << std::endl;
		return FAILURE;
	}
	return SUCCESS;
}