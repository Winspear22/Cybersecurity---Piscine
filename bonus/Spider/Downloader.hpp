/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Downloader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:45:23 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 14:31:19 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __DOWNLOADER_HPP__
#define __DOWNLOADER_HPP__

#include "includes.hpp"

class Downloader
{
public:
    Downloader();
    Downloader(const Downloader &src);
    Downloader &operator=(const Downloader &src);
    ~Downloader();

    std::string curl(const std::string &url);

private:
    static size_t _write_callback(void *contents, size_t size, size_t nmemb,
                                  void *userp);
};

#endif