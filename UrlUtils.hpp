/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UrlUtils.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 23:03:05 by adnen             #+#    #+#             */
/*   Updated: 2026/02/21 23:07:34 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "Spider.hpp"

class UrlUtils
{
    public:
        UrlUtils(void);
        UrlUtils(const UrlUtils &src);
        const UrlUtils &operator=(const UrlUtils &src);
        ~UrlUtils(void);

        static bool    parseArguments(int argc, char **argv, Spider &spider);

    private:
        
};
