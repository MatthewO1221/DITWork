
/*
1.
A famous mathematical problem is: given a graph, find out whether it is possible to color
its vertices using 3 colors (say R,G,B) so that no 2 adjacent vertices have the same
color.

Design a **iterative brute-force** algorithm to solve this problem based on the given
definition.

What is the run-time complexity of your algorihm?

You may use the following pseudo-code constructs:
for each combinatorial object (permutations, tuples, etc
assume you have bool CheckColors() - returns true if assigned colors satisfy the constraint.


2.
Solve the above problem using a **backtracking** algorithm (recursive brute-force).

3.
Your game uses sprites (2-D images), for simplicity it is just one color. Here is an
example â€carâ€ sprite
    *********
   **********
********************
********************
***            ***
your graphics can display either 1x1 or 3x3 squares in one time unit. Problem -
write a backtracking algorithm that accepts a sprite and represents it in the smallest
number of 1x1 and 3x3 squares to optimize the time needed to display the sprite.

3x3 squares MAY overlap - overlapping does not change the color, so for example
***
****
****
 ***
is 2 3x3 squares
111
111*
111*
 ***
and
***
*222
*222
 222
Remember - it should be a high-level pseudo code. Anything longer than 10 lines will
go into unnecessary low-level details and will probably be incorrect.


4.
based on your solution of the previous problem: Explain how to design branch-and-bound
optimization. Make sure to specify how to calculate the bound, whether it is an upper or
lower bound and how branch termination condition works.


5.
    Design an algorithm to rearrange elements of a given array of n real
    numbers so that all its negative elements precede all its positive
    elements. Your algorithm should be both time- and space-efficient.

6.
    Nuts and Bolts.
    You are given a collection if n bolts of different width and n
    corresponding nuts. You are allowed to try a nut and a bolt together, from
    which you can determine whether the nut is larger than the bolt, smaller
    than the bolt, or matches the bolt exactly.  However, there is no way to
    compare two bolts together, or two nuts together. The problem is to match
    each bolt to its nut.  Design an algorithm for this problem with
    average-case efficiency in Theta( n log n ).

*/

// Problem 2

/*

//Use Kickoff Function to start recursion
//Place current color to check into current slot
//CheckColor to see if it works
//Base Case: If all slots full this is solution so exit out
//If it works call recurrent function on all colors in next slot
/If it doesn't work break out of this and go back





*/

// Problem 3

/*

Keep track of best solution so far
Foreach pixel in sprite, use recursion to choose to place either 3x3 or 1x1 square
If placing that square causes the solution to be worse than the current best, backtrack
If placing that square causes the solution to be illegal, backtrack
If you've tried both square sizes already backtrack
Return best solution


*/

// Problem 4

/*


Using lower bound of best possible solution in current branch
Keep track of best solution so far
Foreach pixel in sprite, use recursion to place either 3x3, 1x1 square
If the lower bound of the current stage of the solution is greater than the best solution backtrack
If placing that square causes the solution to be illegal, backtrack
If you've tried both square sizes already backtrack
Return best solution


*/

// Problem 6

/*

//Get pointers to first index of bolt array and last index of nut array
//Loop until nut pointer reaches beginning of nut array
    //Compare nut and bolt at pointers
    //If nut bigger than bolt




*/