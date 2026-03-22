/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetadataEditor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 06:58:16 by adnen             #+#    #+#             */
/*   Updated: 2026/03/22 07:00:20 by adnen            ###   ########.fr       */
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
        void editMetadata(const std::string &filename);
    private:
        std::string _filename;
};

#endif