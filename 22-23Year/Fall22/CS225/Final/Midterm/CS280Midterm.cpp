1.
/*
The best run time of the algorithm occurs when one of the elements in the first row is 0, meaning the algorithm will only run for that row before skipping all the rest, making it O(N) time
The worst case of the algorithm occurs when there are no zeros, in which case the algorithm will have to run for every element in the matrix, making the complexity O(N^2) time
*/


2.
/*
The best case occurs when all the elements in the array are odd, meaning that the remaining work is divided in half every time, making it O(logN) time
The worst case occurs when all the elements in the array are even, in which case the loop will run through every element, making it O(N) time
*/

3.
/*
a: True
b: False
c: True
d: True
e: True
f: False
*/

4.

void sequence(int K, unsigned m)
{
    std::cout << K;

    if (m > 1) //If we're not on the last one to print
    {
        //Calculate the new K
        if (K % 2 == 0)
        {
            K /= 2;
        }
        else
        {
            K = (K * 3) + 1
        }
        
        //Run through again with the new K and one less iteration left
        sequence(K, m - 1);
    }
    
}


5.

//Pass infinity for m when calling function
int min_element_list (Node * p, int m)
{
    if (!p) return m;

    return min_element_list(p->next, min(p->data, m));
}

6.
/*
Assuming the same execution time
In O(1) time it would take 1 second
In O(N) time it would take 2 seconds
In O(N^3) time it would take 
In O(NlogN) time it would take
*/

7.
/*
6
3 6
2 3 6
1 2 3 6, get + next add 1 and 2
3 3 6, get + next, add 3 and 3
6 6, get + next, add 6 and 6
12
5 12
4 5 12, get - next, subtract 4 from 5
1 12, get * next, multiply 1 and 12
12
2 12
7 2 12, get * next, multiply 7 and 2
14 12, get + next, add 14 and 12
26, final answer
*/

8.
/*
5 * (6 * (3 + 2) + 9) - 8, send 5 to output
Output: 5
Stack:
* (6 * (3 + 2) + 9) - 8, stack is empty so * pushed
Output: 5
Stack: *
(6 * (3 + 2) + 9) - 8, push onto stack
Output: 5
Stack: *(
6 * (3 + 2) + 9) - 8, send 6 to output
Output: 56
Stack: *(
* (3 + 2) + 9) - 8, top of stack is ( so push
Output: 56
Stack: *(*
(3 + 2) + 9) - 8, push onto stack
Output: 56
Stack: *(*(
3 + 2) + 9) - 8, send 3 to output
Output: 563
Stack: *(*(
+ 2) + 9) - 8, top of stack is ( so push
Output: 563
Stack: *(*(+
2) + 9) - 8, send 2 to output
Output: 5632
Stack: *(*(+
) + 9) - 8, send + to output
Output: 5632+
Stack: *(*
+ 9) - 8, * has higher precedence to pop it then push +
Output: 5632+*
Stack: *(+
9) - 8, send 9 to output
Output: 5632+*9
Stack: *(+
) - 8, send + to output
Output: 5632+*9+
Stack: *
- 8, push -
Output: 5632+*9+
Stack: *-
8, send 8 to output
Output: 5632+*9+8
Stack: *-
list empty, pop all operators
Output: 5632+*9+*-
Stack: 
*/

9.

/*
A:
Make head of new list

traverse list 1
{
    traverse list 2
    {
        if value in list 1 == value in list 2
        {
            push value onto output list
            break;
        }
    }
}

return output list

This algorithm is O(N^2) time

B:
I would honestly do the same thing for the second problem

C:
Make head of new list
Sort both input lists

traverse list 1
{
    find value of list 1 in list 2, taking into account that they're sorted
    if value is found
    {
        push value onto output list
    }
}

return output list

This algorithm is O(NLogN) time
*/

/*
0 4
1 3
2 2

0 5
1 4
2 3
*/
void reverse(int* array, int n)
{
    if ()
    {
        int storage = array[0];
        array[0] = array[n - 1];
        array[n - 1] = storage;
        reverse(array + 1, n - 1);    
    }
    
    
}

int compare(int* array1, int* array2, int n, int index = 0)
{
        if (index == n)
        {
            return index;
        }
        else if (array1[index] == array2[index])
        {
            return index;
        }
        else if (array1[index] != array2[index])
        {
            return compare(array1, array2, n, index + 1);
        }


        
        return n;
        
}

int product(int x, int y)
{
    if (x == 0 || y == 0)
    {
        return 0;
    }
    else if (y == 1)
    {
        return x;
    }
    
    
    return x + product(x, y - 1);
    
}

int tailproduct(int x, int y)
{
    iterations(true);
    if (x == 0 || y == 0)
    {
        return 0;
    }
    else if (iterations(false) == y)
    {
        return 0;
    }
    else if (iterations(false) < y)
    {
        return x;
    }
    
    
    
    return x + product(x, y);
    
}

int iterations(bool add)
{
    static int iter = 0;
    if (add)
    {
        iter++;
    }
    
    iter++;
    return iter;
}