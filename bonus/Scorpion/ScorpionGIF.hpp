/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionGIF.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:38:51 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 14:56:09 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SCORPION_GIF_HPP__
#define __SCORPION_GIF_HPP__

#include "Scorpion.hpp"

class ScorpionGIF : public Scorpion
{
public:
    ScorpionGIF();
    ScorpionGIF(const ScorpionGIF &src);
    ScorpionGIF &operator=(const ScorpionGIF &src);
    virtual ~ScorpionGIF();

private:
    void parseSpecific(std::ifstream &file) override;
};

#endif