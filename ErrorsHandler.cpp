/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorsHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 19:09:00 by adnen             #+#    #+#             */
/*   Updated: 2026/01/27 19:10:58 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

bool	print_error(const std::string& error_msg)
{
	std::cerr << BOLD_RED << error_msg << RESET << std::endl;
	return FAILURE;
}

bool	print_success(const std::string& success_msg)
{
	std::cout << BOLD_GREEN << success_msg << RESET << std::endl;
	return SUCCESS;
}

bool	print_info(const std::string& info_msg)
{
	std::cout << BOLD_BLUE << info_msg << RESET << std::endl;
	return INFO;
}