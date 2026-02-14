/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionPNG.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:38:56 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 14:56:13 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SCORPION_PNG_HPP__
#define __SCORPION_PNG_HPP__

#include "Scorpion.hpp"

class ScorpionPNG : public Scorpion
{
public:
    ScorpionPNG();
    ScorpionPNG(const ScorpionPNG &src);
    ScorpionPNG &operator=(const ScorpionPNG &src);
    virtual ~ScorpionPNG();

private:
    void parseSpecific(std::ifstream &file) override;
};

#endif