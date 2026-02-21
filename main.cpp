/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 19:49:01 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 23:09:12 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "Spider.hpp"
#include "UrlUtils.hpp"

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 7)
    {
        std::cout << "Error : number of arguments must be between 2 and 5." << std::endl;
        std::cout << "Possible Usage : ./spider -r -l [N] -p [PATH] URL" << std::endl;
        return (EXIT_FAILURE);
    }
    Spider spider;
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != 0)
        std::cout << "Error: " << curl_easy_strerror(global_res) << std::endl;
    if (UrlUtils::parseArguments(argc, argv, spider) == FAILURE)
        return (EXIT_FAILURE);
    spider.run();
    curl_global_cleanup();
    return (EXIT_SUCCESS);
}