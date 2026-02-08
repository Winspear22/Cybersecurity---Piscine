/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scorpion.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:50:21 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 22:46:24 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SCORPION__
#define __SCORPION__

#include "../Spider/includes.hpp"

class Scorpion
{
public:
  Scorpion();
  ~Scorpion();
  Scorpion(const Scorpion &src);
  Scorpion &operator=(const Scorpion &src);

		void		FileIdentification(const std::string &filename);

private:
		void		_fileAnalysis(const std::string &fileName);
		void		_dateAnalysis(const struct stat &st);
		void		_weightAnalysis(const struct stat &st);
		void		_findExifBlock(std::ifstream &file);
		void		_parseTiff(std::ifstream &file);
		void		_readTagValue(std::ifstream &file, unsigned char *tagEntry, 
						long tiffStart, bool isLittleEndian);
};

#endif