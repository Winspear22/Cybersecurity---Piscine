/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 19:49:01 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 21:02:10 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "Spider.hpp"


int main(int argc, char **argv)
{
    if (argc < 2 || argc > 7)
    {
        std::cout << "Error : number of arguments must be between 2 and 5." << std::endl;
        std::cout << "Possible Usage : ./spider -r -l [N] -p [PATH] URL" << std::endl;
        return (EXIT_FAILURE);
    }
    int opt;
    Spider spider;
    while ((opt = getopt(argc, argv, "rl:p:")) != -1)
    {
        switch (opt)
        {
            case 'r':
                spider.setIsRecursive(true);
                break;
            case 'l':
                spider.setDepthNumber(std::stoi(optarg));
                break ;
            case 'p':
                spider.setPathOfDownload(optarg);
                break ;
            case '?':
                std::cerr << "Error, wrong set of options." << std::endl;
                return EXIT_FAILURE;
        }
        
    }
    return (0);
}