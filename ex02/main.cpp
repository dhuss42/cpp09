/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:26:09 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/22 16:25:18 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int	main(int argc, char **argv)
{
	if (argc < 2)
		PmergeMe::err("invalid number of arguments");
	else
	{
		// std::cout << "\033[33m=============Default=============\033[0m" << std::endl;
		PmergeMe pmM;
		pmM.execute(argc, argv);

		// std::cout << "\033[33m=============copy=============\033[0m" << std::endl;
		// PmergeMe copy(pmM);
		// copy.execute(argc, argv);

		// std::cout << "\033[33m=============assign=============\033[0m" << std::endl;
		// PmergeMe assign;
		// assign = pmM;
		// assign.execute(argc, argv);
	}
}

// add headers
// check private attributes inside hpp file
//	- get rid of _leftCounter
