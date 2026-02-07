/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:27:51 by adnen             #+#    #+#             */
/*   Updated: 2026/02/07 17:19:35 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "spider.hpp"
#include "Saver.hpp"

int main(int argc, char **argv)
{
	// 1. INIT GLOBAL (L'environnement)
    // CURL_GLOBAL_ALL : Charge tout (SSL pour le HTTPS, gestionnaire de mémoire, etc.)
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
	if (global_res != 0)
		std::cout << RED << "Error: " << curl_easy_strerror(global_res) << RESET << std::endl;
	// 2. LOGIQUE SPIDER (Juste le minimum pour tester)
    if (argc > 1 && argc < 7)
    {        
        Spider spider;              // Création
        spider.addArgumentsToVector(argv); // Configuration
		if (spider.argsParser() == FAILURE)
			return (1);
        spider.run();               // Action !
    }
    else
        std::cout << "Usage: ./spider URL" << std::endl;
	// 3. CLEANUP GLOBAL
    // On éteint le moteur avant de partir (INDISPENSABLE)
    curl_global_cleanup();
	return 0;
}