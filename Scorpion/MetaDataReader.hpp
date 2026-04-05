/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetaDataReader.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 23:52:31 by adnen             #+#    #+#             */
/*   Updated: 2026/04/05 11:37:29 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __METADATAREADER_HPP__
#define __METADATAREADER_HPP__

#include "includes.hpp"

class MetaDataReader
{
	public:
  		MetaDataReader();
  		~MetaDataReader();
  		const MetaDataReader &operator=(const MetaDataReader &other);
  		MetaDataReader(const MetaDataReader &other);
		
		static void displayMetadata(const std::string &filename);

	private:
  		static std::string _formatSize(uintmax_t bytes);
  		static std::string _formatDate(const std::string &filename);
  		static std::string _formatCreationDate(const std::string &filename);
  		static std::string _formatPermissions(std::filesystem::perms p);
};

#endif
