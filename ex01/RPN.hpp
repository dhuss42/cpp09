/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:40:24 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/26 13:09:38 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

#include <stack>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

typedef enum e_errors
{
	E_ARGS,
	E_DIVISIONZERO,
	E_INVALID,
	E_EMPTY,
} t_errors;

class RPN
{
	private:
		int	_countOperands;
		int	_countOperators;
		std::stack<float> _operands;
	public:
		RPN();
		RPN(const RPN& src);
		RPN& operator=(const RPN& other);
		~RPN();

		float	reversePolishNotation(const std::string& str);
		void	handleOperator(char op);
		void	handleDigit(std::string digit);
		static void	err(t_errors err);
};


#endif