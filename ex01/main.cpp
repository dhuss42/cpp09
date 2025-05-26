/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:40:16 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/26 13:12:19 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
		RPN::err(E_ARGS);
	else
	{
		RPN rpn;
		std::cout << rpn.reversePolishNotation(argv[1]) << std::endl;
	}
	return (0);
}