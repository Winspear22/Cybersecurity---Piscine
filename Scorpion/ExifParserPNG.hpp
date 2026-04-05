/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExifParserPNG.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:29:00 by adnen             #+#    #+#             */
/*   Updated: 2026/04/05 11:39:38 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __EXIFPARSERPNG_HPP__
#define __EXIFPARSERPNG_HPP__

#include "ExifParser.hpp"

class ExifParserPNG : public ExifParser
{
	public:
		ExifParserPNG();
		~ExifParserPNG();
		ExifParserPNG(const ExifParserPNG &other);
		const ExifParserPNG &operator=(const ExifParserPNG &other);

		void parse(const std::vector<unsigned char> &data);
};

#endif
