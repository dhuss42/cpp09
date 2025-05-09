/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:37:08 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/09 15:37:10 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error could not opn file." << std::endl;
		return (-1);
	}
	else
	{
		BitcoinExchange btc(argv[1]);
		std::cout << std::fixed << std::setprecision(2);
		btc.mapDataBase();
		btc.parsing();
	}
	return (0);
}