/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spider.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:12:43 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 20:50:17 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SPIDER_HPP__
#define __SPIDER_HPP__

# include "includes.hpp"

class Spider
{
	public:
		Spider(void);
		Spider(const Spider &src);
		const Spider &operator=(const Spider &src);
		~Spider(void);

		bool			getIsRecursive(void);
		void			setIsRecursive(bool isRecursive);
		int				getDepthNumber(void);
		void			setDepthNumber(int nb);
		std::string		getPathOfDownload(void);
		void			setPathOfDownload(std::string path);

	private:
		bool			_isRecursive;
		int				_depthNumber;
		std::string		_pathOfDownload;
};

#endif