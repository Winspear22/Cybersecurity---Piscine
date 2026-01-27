/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:16:38 by adnen             #+#    #+#             */
/*   Updated: 2026/01/27 19:15:14 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ARACHNIDA_H__
#define __ARACHNIDA_H__

# include <iostream>
# include <vector>
# include <string>
# include <algorithm>
# include <curl/curl.h>
# include <fstream>
# include <set>
# include <regex>
# include <climits>

# define SUCCESS	0
# define FAILURE	1
# define INFO		2

// --- RESET (Indispensable pour arrêter la couleur) ---
#define RESET   "\033[0m"

// --- COULEURS NORMALES ---
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// --- COULEURS GRAS (Plus visible, style "Matrix") ---
#define BOLD_BLACK   "\033[1;30m"
#define BOLD_RED     "\033[1;31m"
#define BOLD_GREEN   "\033[1;32m"
#define BOLD_YELLOW  "\033[1;33m"
#define BOLD_BLUE    "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN    "\033[1;36m"
#define BOLD_WHITE   "\033[1;37m"


bool print_error(const std::string& msg);
bool print_success(const std::string& msg);
bool print_info(const std::string& msg);

#endif