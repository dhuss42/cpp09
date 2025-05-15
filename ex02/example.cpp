
#include <iostream>
#include <vector>

void	printContainer(std::vector<int> vect)
{
	for (auto it = vect.begin(); it < vect.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

void	recursiveCallRef(std::vector<int> &vect, int counter)
{
	std::cout << "run [" << counter << "]" << std::endl;
	if (counter < 3)
	{
		vect.push_back(2 + counter);
		recursiveCallRef(vect, counter +1);
	}
	if (counter == 2)
		vect.push_back(1000);
}

void	recursiveCallPtr(std::vector<int> *vect, int counter)
{
	std::cout << "run [" << counter << "]" << std::endl;
	if (counter < 3)
	{
		vect->push_back(2 + counter);
		recursiveCallPtr(vect, counter +1);
	}
	if (counter == 2)
		vect->push_back(1000);
}

int main()
{
	std::vector<int> vect;
	int	counter = 1;

	vect.push_back(2);
	vect.push_back(23);
	vect.push_back(4);
	vect.push_back(1);
	std::cout << "before call " << std::endl;
	printContainer(vect);
	// recursiveCallRef(vect, counter);
	recursiveCallPtr(&vect, counter);
	std::cout << "after call " << std::endl;
	printContainer(vect);
}