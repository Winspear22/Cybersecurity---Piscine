/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UrlHelper.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:52:16 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 14:18:32 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __URLHELPER_HPP__
#define __URLHELPER_HPP__

#include "includes.hpp"

class UrlHelper
{
public:
    UrlHelper();
    UrlHelper(const UrlHelper &src);
    UrlHelper &operator=(const UrlHelper &src);
    ~UrlHelper();

    std::string _resolve_url(const std::string &base_url,
                             const std::string &link_url);
    bool _is_valid_extension(const std::string &url);

private:
};

#endif
