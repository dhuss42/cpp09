/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:26:09 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/21 15:52:47 by dhuss            ###   ########.fr       */
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

// 80 45 48 25 99 30 74 98 13 53 3 81 13 74 21 88 61 28 84 2 25 84 6 18 46
//-> comparison 65 is different for vector and deque

// ontainer:       | 6  | 18  | 25  | 48  | 13  | 53  | 28  | 61  | 13  | 74  | 45  | 80  | 3  | 81  | 2  | 84  | 25  | 84  | 21  | 88  | 74  | 98  | 30  | 99  | 46 

// mainChain:       | 6  | 18  | 48  | 53  | 61  | 74  | 80  | 81  | 84  | 84  | 88  | 98  | 99 
// mainChain:       | b1 | a1  | a2  | a3  | a4  | a5  | a6  | a7  | a8  | a9  | a10  | a11  | a12 

// the pend:        | 25  | 13  | 28  | 13  | 45  | 3  | 2  | 25  | 21  | 74  | 30  | 46 
// the pend:        | b2  | b3  | b4  | b5  | b6  | b7 | b8 | b9  | b10  | b11  | b12  | b13

// vector:
// mainChain after Insetion:        | 2  | 6  | 13  | 13  | 18  | 21  | 25  | 25  | 28  | 48  | 53  | 61  | 74  | 74  | 80  | 81  | 84  | 84  | 88  | 98  | 99 

// pend after Insetion:             | 45  | 3  | 30  | 46 
// startPend: 3
// endPend: 45
// leftCounter: 4
// RightCounter: 0
// start main chain: 2
// end main chain: 80
// Ax - 1 is: 80
// the range between elements in mainChain is: 14

// deque: 
// mainChain after Insetion:        | 2  | 6  | 13  | 13  | 18  | 21  | 25  | 25  | 28  | 48  | 53  | 61  | 74  | 74  | 80  | 81  | 84  | 84  | 88  | 98  | 99 

// pend after Insetion:             | 45  | 3  | 30  | 46 
// startPend: 45
// endPend: 45
// leftCounter: 4
// RightCounter: 0
// start main chain: 2
// end main chain: 80
// Ax - 1 is: 80
// the range between elements in mainChain is: 14


//  2  | 6  | 13  | 13  | 18  | 21  | 25  | 25  | 28  | 45  | 48  | 53  | 61  | 74  | 74  | 80  | 81  | 84  | 84  | 88  | 98  | 99 

// pend after Insetion:             | 3  | 30  | 46 

// mainChain after Insetion:        | 2  | 6  | 13  | 13  | 18  | 21  | 25  | 25  | 28  | 30  | 45  | 45  | 46  | 48  | 53  | 61  | 74  | 74  | 80  | 81  | 84  | 84  | 88  | 98  | 99 
// -> three disapears and turns into 45 the value before 3 which is b6