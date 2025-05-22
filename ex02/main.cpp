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
	else if (argc > INT_MAX)
		PmergeMe::err("number of arguments is too high"); // test this
	else
	{
		PmergeMe pmM;
		pmM.execute(argc, argv);
	}
}

// handle Input higher than INT_MAX
// check more

// add headers
// check private attributes inside hpp file
