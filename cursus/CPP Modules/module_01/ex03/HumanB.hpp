/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcakmako <tcakmako@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 23:17:00 by tcakmako          #+#    #+#             */
/*   Updated: 2022/11/28 13:37:16 by tcakmako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMAN_B_HPP
# define HUMAN_B_HPP

# include "Weapon.hpp"
# include <string>
# include <iostream>

class	HumanB
{
	private:
		std::string name;
		Weapon		*weapon;

	public:
		HumanB(std::string n);
		
		void	attack(void);
		void	setWeapon(Weapon &);
};

#endif
