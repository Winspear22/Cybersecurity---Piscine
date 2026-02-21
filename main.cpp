/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 19:49:01 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 22:53:53 by adnen            ###   ########.fr       */
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
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != 0)
        std::cout << "Error: " << curl_easy_strerror(global_res) << std::endl;
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
                catch (const std::invalid_argument& e)
                {
                    std::cerr << "Error: Depth '-l' must be a valid number (e.g. -l 5)." << std::endl;
                    return (EXIT_FAILURE);
                }
                catch (const std::out_of_range& e)
                {
                    std::cerr << "Error: Depth number is too large." << std::endl;
                    return (EXIT_FAILURE);
                }
                break;
            case 'p':
                spider.setPathOfDownload(optarg);
                break ;
            case '?':
                std::cerr << "Error, wrong set of options." << std::endl;
                return EXIT_FAILURE;
        }
    }
    if (optind < argc) 
        spider.setUrl(argv[optind]);
    else 
    {
        std::cerr << "Error, Missing URL." << std::endl;
        return (EXIT_FAILURE);
    }
    spider.run();
    curl_global_cleanup();
    return (EXIT_SUCCESS);
}