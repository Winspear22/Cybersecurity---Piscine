/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UrlParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 17:28:11 by adnen             #+#    #+#             */
/*   Updated: 2026/02/22 17:28:19 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __URL_PARSER_HPP__
#define __URL_PARSER_HPP__

#include "includes.hpp"

class UrlParser
{
	public:
		UrlParser(void);
		UrlParser(const UrlParser &src);
		const UrlParser &operator=(const UrlParser &src);
		~UrlParser(void);

	private:
};

#endif
