/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExifParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:03:45 by adnen             #+#    #+#             */
/*   Updated: 2026/03/03 00:04:11 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __EXIFPARSER_HPP__
#define __EXIFPARSER_HPP__

#include "includes.hpp"

class ExifParser
{
	public:
		ExifParser();
		~ExifParser();
		const ExifParser &operator=(const ExifParser &other);
		ExifParser(const ExifParser &other);

	private:

};

#endif