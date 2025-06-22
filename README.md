# cpp09

This module is focused on introducing and practicing the use of the Standard Template Library (STL) in C++. Each exercise in this module must be solved using STL containers, with a strict limitation: once a container is used in one exercise, it cannot be reused in the others. Exercises 00 and 01 require the use of one container each, while Exercise 02 requires the use of two different containers.

## ex00

In this exercise, I built a program named btc that calculated the value of Bitcoin on a given date using historical data from a provided CSV file. I used the std::map container to store the exchange rates, mapping each date to its corresponding value. For every line in the input file (formatted as date | value), I parsed the data, validated the date and value, located the exact or closest earlier date in the map, and computed the result by multiplying the input value with the appropriate exchange rate.  

## ex01

In this exercise, I built a program named RPN that evaluated mathematical expressions written in Reverse Polish Notation. I used the std::stack container to manage the operands during computation. As I parsed the input string, I pushed digits between 0 and 9 onto the stack. When I encountered an operator, I popped the top two values from the stack, applied the operation, and pushed the result back. This approach correctly handled expressions like "2 2 +", "3 4 * 5 -", and "5 1 2 + 4 * + 3 -".

## ex02

This was the most complex of the three exercises. I built a program named PmergeMe that sorted a sequence of positive integers using the Ford-Johnson algorithm, also known as merge-insert sort. I chose std::vector and std::deque as the two required containers since they offer similar interfaces and behavior, which allowed me to implement much of the logic in generic functions. The main difference between them lies in their memory layout—vector uses contiguous memory while deque uses segmented memory—which later proved to impact performance. The Ford-Johnson algorithm is notable for requiring the fewest possible comparisons to sort a sequence. For example, sorting 21 numbers takes only 66 comparisons

The sorting process consisted of several steps. First, I implemented a recursive algorithm that began by comparing and conditionally swapping each pair of adjacent digits. Then, I grouped these digits into pairs and moved up one recursion level, treating each pair as a unit. I then compared the trailing digit of each pair and swapped them accordingly. This process repeated—doubling the size of each group at each level—until recursion could no longer form two valid pairs. At that point, the algorithm began resolving downward, back to the base level.

In the second step, I split the reorganized sequence into two containers: the main chain and the pend. The pairs were indexed as b1, a1, b2, a2, etc., where all b elements except b1 went into the pend chain, and the rest formed the main chain.

The third step involved inserting the pend elements back into the main chain using binary insertion. The order of insertion followed the Jacobsthal sequence, starting from 3 and inserting elements in descending index order (e.g., from 3 down to 2, then 5 down to 4, then 11 down to 6, and so on). This strategy minimizes comparisons in cases where an inserted element falls to the left of its logical bound pair (ax bx), further optimizing the process.

Finally, as the recursion unwound, each layer was resolved, resulting in a fully sorted sequence. I measured the time taken for each container to complete the sort, and vector consistently outperformed deque—likely due to its contiguous memory
