/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spider.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 20:12:43 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 20:22:31 by adnen            ###   ########.fr       */
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

	private:
};

#endif