/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImageDownloader.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 17:49:38 by adnen             #+#    #+#             */
/*   Updated: 2026/02/26 17:49:45 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IMAGE_DOWNLOADER_HPP__
#define __IMAGE_DOWNLOADER_HPP__

#include "includes.hpp"

class ImageDownloader
{
	public:
		ImageDownloader(void);
		ImageDownloader(const ImageDownloader &src);
		const ImageDownloader &operator=(const ImageDownloader &src);
		~ImageDownloader(void);

		void downloadImage(std::string url);

	private:
		std::string _url;
};

#endif
