/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScorpionWEBP.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adnen <adnen@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:39:00 by adnen             #+#    #+#             */
/*   Updated: 2026/02/14 14:56:20 by adnen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SCORPION_WEBP_HPP__
#define __SCORPION_WEBP_HPP__

#include "Scorpion.hpp"

class ScorpionWEBP : public Scorpion
{
	public:
		ScorpionWEBP();
		ScorpionWEBP(const ScorpionWEBP &src);
		ScorpionWEBP &operator=(const ScorpionWEBP &src);
		virtual ~ScorpionWEBP();
	private:
		void parseSpecific(std::ifstream &file) override;
};

#endif