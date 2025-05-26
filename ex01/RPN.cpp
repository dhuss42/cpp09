/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:40:50 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/26 13:31:42 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

//----------------Constructors-----------------//

/*----------------------*/
/* Constructor			*/
/*----------------------*/
RPN::RPN() : _countOperands(0), _countOperators(0)
{
}

/*----------------------*/
/* Copy Constructor		*/
/*----------------------*/
RPN::RPN(const RPN& src) : _countOperands(src._countOperands), _countOperators(src._countOperators), _operands(src._operands)
{
}

/*------------------------------------------*/
/* Overloaded Assignment operator			*/
/*------------------------------------------*/
RPN& RPN::operator=(const RPN& other)
{
	if (this != &other)
	{
		_countOperands = other._countOperands;
		_countOperators = other._countOperators;
		_operands = other._operands;
	}
	return (*this);
}

/*----------------------*/
/* Destructor			*/
/*----------------------*/
RPN::~RPN()
{
}

//----------------helpers-----------------//

/*--------------------------------*/
/* returns true if valid operator */
/*--------------------------------*/
bool	isOperator(char op)
{
	return (op == '+' || op == '-' || op == '/' || op == '*');
}

/*----------------------------------------------------------*/
/* manages calculations of a and b based on passed operator */
/*----------------------------------------------------------*/
float	calculate(float a, float b, char op)
{
	if (op == '+')
		return (a + b);
	else if (op == '-')
		return (a - b);
	else if (op == '*')
		return (a * b);
	else
	{
		if (a == 0)
			RPN::err(E_DIVISIONZERO);
		return (a / b);
	}
}

//----------------Member Methods-----------------//

/*----------------------------------*/
/* stores top values as floats		*/
/* does calculation					*/
/* pushes result on stack			*/
/*----------------------------------*/
void	RPN::handleOperator(char op)
{
	float a, b = 0;

	b = _operands.top();
	_operands.pop();
	a = _operands.top();
	_operands.pop();
	_operands.push(calculate(a, b, op));
	_countOperators++;
}

/*----------------------------------*/
/* converts digit to float			*/
/* pushes digit on stack			*/
/*----------------------------------*/
void	RPN::handleDigit(std::string digit)
{
	float	n = std::stof(digit);
	_operands.push(n);
	_countOperands++;
}

/*----------------------------------------------------------*/
/* basic parsing											*/
/* reads from ss and stores in token until delimiter or EOF */
/* checks if digit											*/
/* checks if operator										*/
/* checks if number of digits and oeprators match up		*/
/* returns result											*/
/*----------------------------------------------------------*/
float	RPN::reversePolishNotation(const std::string& str)
{
	while (!_operands.empty())
		_operands.pop();
	if (str.empty())
	err(E_EMPTY);
	if (str.length() == 1)
	err(E_INVALID);
	std::stringstream	ss(str);
	std::string			token;
	while (getline(ss, token, ' '))
	{
		if (token.length() != 1)
		err(E_INVALID);
		if (isdigit(token[0]))
		handleDigit(token);
		else if (isOperator(token[0]) && _operands.size() > 1)
		handleOperator(token[0]);
		else
		err(E_INVALID);
	}
	if (_countOperands - _countOperators != 1)
	err(E_INVALID);
	_countOperands = 0;
	_countOperators = 0;
	return (_operands.top());
}

/*--------------------------*/
/* handles errors			*/
/*--------------------------*/
void	RPN::err(t_errors err)
{
	std::cerr << "\033[31mError: ";
	if (err == E_ARGS)
		std::cerr << "Invalid number of arguments!";
	else if (err == E_DIVISIONZERO)
		std::cerr << "Trying to divide through zero!";
	else if (err == E_INVALID)
		std::cerr << "Invalid input!";
	else if (err == E_EMPTY)
		std::cerr << "Input String is empty!";
	std::cerr << "\033[0m" << std::endl;
	exit(EXIT_FAILURE);
}
