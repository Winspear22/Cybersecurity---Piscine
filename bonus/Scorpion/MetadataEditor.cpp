/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetadataEditor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 06:58:13 by adnen             #+#    #+#             */
/*   Updated: 2026/03/22 07:00:50 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MetadataEditor.hpp"

MetadataEditor::MetadataEditor()
{
	_filename = "";
	std::cout << "MetadataEditor constructor called" << std::endl;
}

MetadataEditor::~MetadataEditor()
{
	std::cout << "MetadataEditor destructor called" << std::endl;
}

MetadataEditor &MetadataEditor::operator=(const MetadataEditor &other)
{
    if (this != &other)
        _filename = other._filename;
	std::cout << "MetadataEditor copy assignment operator called" << std::endl;
    return (*this);
}

MetadataEditor::MetadataEditor(const MetadataEditor &src)
{
	std::cout << "MetadataEditor copy constructor called" << std::endl;
    *this = src;
}

const std::string &MetadataEditor::getFilename() const
{
    return (_filename);
}

void MetadataEditor::setFilename(const std::string &filename)
{
    _filename = filename;
}

void MetadataEditor::editMetadata(const std::string &filename)
{
    _filename = filename;
}
