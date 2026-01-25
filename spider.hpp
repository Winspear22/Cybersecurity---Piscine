/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spider.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:27:43 by adnen             #+#    #+#             */
/*   Updated: 2026/01/26 00:41:49 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPIDER_HPP
#define SPIDER_HPP

#include "includes.hpp"

class Spider
{
	public:
		Spider();
		Spider(const Spider &src);
		Spider &operator=(const Spider &src);
		~Spider();
	private:
		
};

#endif