/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scorpion.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:50:21 by adnen             #+#    #+#             */
/*   Updated: 2026/02/08 15:48:43 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SCORPION__
#define __SCORPION__

# include "../Spider/includes.hpp"

class Scorpion
{
    public:
        Scorpion();
        ~Scorpion();
        Scorpion(const Scorpion &src);
        Scorpion &operator=(const Scorpion &src);

		void 	FileIdentification(const std::string& filename);

    private:
        
};

#endif