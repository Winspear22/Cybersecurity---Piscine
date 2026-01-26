/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Saver.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:56:16 by adnen             #+#    #+#             */
/*   Updated: 2026/01/26 13:27:39 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SAVER_HPP__
#define __SAVER_HPP__

# include "includes.hpp"

class Saver
{
	public:
		Saver();
		Saver(const Saver &src);
		Saver &operator=(const Saver &src);
		~Saver();

		void save_file(const std::string& data, const std::string& url);

	private:
		std::string _extract_filename(const std::string& url);

};

#endif