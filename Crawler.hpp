/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Crawler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 20:49:57 by adnen             #+#    #+#             */
/*   Updated: 2026/03/02 20:50:15 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CRAWLER_HPP__
#define __CRAWLER_HPP__

#include "includes.hpp"

class Crawler
{
	public:
		Crawler(void);
		Crawler(const Crawler &src);
		const Crawler &operator=(const Crawler &src);
		~Crawler(void);

	private:

};

#endif