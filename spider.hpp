/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spider.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:27:43 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 00:12:33 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SPIDER_HPP__
#define __SPIDER_HPP__

#include "includes.hpp"
#include "Saver.hpp"

class Spider
{
	public:
		Spider();
		Spider(const Spider &src);
		Spider &operator=(const Spider &src);
		~Spider();

		void						addArgumentsToVector(char **argv);
		bool						argsParser(void);
		void						run();

	private:
		Saver											_saver;
		std::string										_start_url;
		std::set<std::string>							_image_urls;
		std::vector<std::string>						_args;
		std::vector<std::pair<std::string, int>>		_url_queue;
		std::set<std::string>                       	_visited_urls;
		bool											_recursive; //-r
		int												_max_depth; //-l
		std::string										_output_dir_path; //-p
		int												_invalid_images_count;
		
		bool											_is_safe_to_scan(std::string& url);
		std::string										_request(const std::string& url);
		static size_t									_write_callback(void *contents, size_t size, size_t nmemb, void *userp);
		void											_parse_html(const std::string& html);
		void											_parse_links(const std::string& html_content, int current_depth);
		std::string										_resolve_url(const std::string& base_url, const std::string& link_url);
		bool											_is_valid_extension(const std::string& url);
};

#endif