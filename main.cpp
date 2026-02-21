/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 19:49:01 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 20:46:29 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"


int main(int argc, char **argv)
{
    if (argc < 2 || argc > 7)
    {
        std::cout << "Error : number of arguments must be between 2 and 5." << std::endl;
        std::cout << "Possible Usage : ./spider -r -l [N] -p [PATH] URL" << std::endl;
        return (EXIT_FAILURE);
    }
    int opt;
    while (opt = getopt(argc, argv, "rl:p:") != -1)
    {
        return ;
    }
    return (0);
}