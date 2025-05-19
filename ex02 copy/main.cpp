/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:26:09 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/14 10:03:53 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

// check for argc > INT_MAX
int	main(int argc, char **argv)
{
	if (argc < 2)
		PmergeMe::err("invalid number of arguments");
	else
	{
		PmergeMe pmM;
		pmM.execute(argc, argv);
	}
}

// handle Input higher than INT_MAX
// check with duplicates
// check with deque
//	problem with deque probably the way iterators are advanced inside the algorithm