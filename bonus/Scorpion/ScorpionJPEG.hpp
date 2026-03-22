/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionJPEG.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:02:36 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 15:03:00 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SCORPION_JPEG_HPP__
#define __SCORPION_JPEG_HPP__

#include "Scorpion.hpp"

class ScorpionJPEG : public Scorpion
{
public:
    ScorpionJPEG();
    ScorpionJPEG(const ScorpionJPEG &src);
    ScorpionJPEG &operator=(const ScorpionJPEG &src);
    virtual ~ScorpionJPEG();

private:
    void parseSpecific(std::ifstream &file) override;
};

#endif