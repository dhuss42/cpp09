/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:37:08 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/12 14:13:35 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
		BitcoinExchange::err(E_ARGS, "", true);
	else
	{
		BitcoinExchange btc(argv[1]);
		btc.mapDataBase();
		btc.extractInput();
	}
	return (0);
}