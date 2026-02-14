/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scorpion.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:50:21 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 15:15:44 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SCORPION__
#define __SCORPION__

#include "../Spider/includes.hpp"

class Scorpion
{
public:
    Scorpion();
    virtual ~Scorpion();
    Scorpion(const Scorpion &src);
    Scorpion &operator=(const Scorpion &src);

    static void FileIdentification(const std::string &filename);

protected:
    void _fileAnalysis(const std::string &fileName);
    void _dateAnalysis(const struct stat &st);
    void _weightAnalysis(const struct stat &st);
    void _parseTiff(std::ifstream &file);
    void _parseIFD(std::ifstream &file, long tiffStart, bool isLittleEndian,
                   unsigned long ifdOffset);
    void _readTagValue(std::ifstream &file, unsigned char *tagEntry,
                       long tiffStart, bool isLittleEndian);
    virtual void parseSpecific(std::ifstream &file) = 0;
};

#endif