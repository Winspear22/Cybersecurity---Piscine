/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetadataEditor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 06:58:16 by adnen             #+#    #+#             */
/*   Updated: 2026/03/22 07:19:22 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __METADATA_EDITOR_HPP
#define __METADATA_EDITOR_HPP

#include "../Spider/includes.hpp"

class MetadataEditor
{
	public:
    	MetadataEditor();
    	~MetadataEditor();
    	MetadataEditor &operator=(const MetadataEditor &other);
    	MetadataEditor(const MetadataEditor &src);

    	const std::string &getFilename() const;
    	void setFilename(const std::string &filename);
    	static bool deleteMetadata(const std::string &filename);
    	static std::string setFormat(const std::string &filename);
    	std::string getFormat(void) const;

	private:
    	std::string _filename;
    	std::string _format;
};

#endif