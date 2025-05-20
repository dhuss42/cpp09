/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:26:09 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/20 15:13:23 by dhuss            ###   ########.fr       */
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


// mainChain:       | 113  | 157  | 287  | 397
// 						b1		a1	a2		a3
// the pend:        | 267  | 253  | 291
// 						b2	b3		b4
// => // mainChain:       | 113  | 157  | 253 | 287  | 397
// 							b1		a1		b3	a2		a3
// -> first time a2 equals b3 index + elementSize
// -> if inserted to the left of a2 the range must remain the same
// -> if inserted to the right end = a2 - elementSize


// 253 -> 287 is inserted to the left => valdi range for b2 is 3
//

// the range inside the mainChain stays the same if the value bx is smaller than ax -1
//	-> keep track with number of insertions
// the range of mainChain shrinks when bx is inserted behind ax - 1
// -> keep track with number of insertions to the right of ax

//