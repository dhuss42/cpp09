/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:26:09 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/19 15:55:44 by dhuss            ###   ########.fr       */
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

//	251 6 183 286 256 291 364 335 43 19
// => 23 comparisons but it should be 22
// _> the problem is the logic when calculating the end point in the main, it should be one earlier

// mainChain:       | 6 251  | 183 286  | 335 364
//						b1		a1			a2
// the pend:        | 256 291  | 19 43
//						b2			b3

//	1. 291 > 251
//	2. 291 > 286 => here should stop because we know a2 > b2
//
// when insertions is not 0 maybe the formula is not correct
// the position in main cannot exceed begining + number of insertions ...
// 3 jacobsthal -> original a3 is: begining + jacobsthal * elementsize -1
// 1 insertion -> original b2 is for sure one element before this
// -> in this case it must be - insertions
