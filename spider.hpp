/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spider.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:27:43 by adnen             #+#    #+#             */
/*   Updated: 2026/01/26 11:53:19 by adnen            ###   ########.fr       */
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

		void parse_arguments(const std::vector<std::string>& args);
		void run();
	private:
		std::string _start_url;
		std::string _request(const std::string& url);
		static size_t _write_callback(void *contents, size_t size, size_t nmemb, void *userp);
		
};

#endif