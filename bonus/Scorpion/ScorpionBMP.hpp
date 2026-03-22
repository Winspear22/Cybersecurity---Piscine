/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionBMP.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:36:05 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 15:17:19 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SCORPION_BMP_HPP__
#define __SCORPION_BMP_HPP__

#include "Scorpion.hpp"

class ScorpionBMP : public Scorpion
{
public:
    ScorpionBMP();
    ScorpionBMP(const ScorpionBMP &src);
    ScorpionBMP &operator=(const ScorpionBMP &src);
    virtual ~ScorpionBMP();

private:
    void parseSpecific(std::ifstream &file) override;
};

#endif