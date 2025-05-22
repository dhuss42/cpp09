/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:26:09 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/22 13:50:49 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

// check for argc > INT_MAX
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
// check with duplicates
// check more

// add headers
// check private attributes inside hpp file

// 25 91 75 13 91 98 50 8 88 63 17 20 77 2 5 6 21