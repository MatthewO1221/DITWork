// Problem 1

/*

SolveQueenProblem(n)
{

    foreach possible position of queens (combination)
        Call CheckAttacks() on the solution
        If it passes add it to list of solutions


    return list of solutions
}



*/

// Problem 2

/*

FindLongestSubsequence(sequence, subsequence, curIndex, longest)
{
    if curIndex is = to total length of sequence
    {
        return;
    }
    Add value at sequence[curIndex] to subsequence
    Recursively call this function with length + 1

    Remove value just added to subsequence
    Recursively call this function with length + 1

    if is_increasing(subsequence) && subsequence is longest found
    {
        longest = subsequence
    }

    return;
}





*/

// Problem 3

/*


FindLongestSubsquence(sequence, subsequence, curIndex, longest)
{
    if curIndex is = to total length of sequence
    {
        return;
    }

    Calculate upperbound = subsequence current length + remaining length of sequence
    if upperbound < length of longest subsequence
    {
        return;
    }

    Add value at sequence[curIndex] to subsequence
    Recursively call this function with length + 1

    Remove value just added to subsequence
    Recursively call this function with length + 1

    if is_increasing(subsequence) && subsequence is longest found
    {
        longest = subsequence
    }

    return;
}

*/

// Problem 4

/*



*/

// Problem 5

/*

FindIndex(array, &index)
{

    Left array = empty array half length of given
    Right array = empty array half length of given

    Populate left array with left half of given and right with right half

    if (value at left array first index < the index)
    {
        FindIndex(left array, &index)
    }

    if (value at right array first index < the index)
    {
        FindIndex(right array, &index)
    }

    if (value at left array first index is equal to index)
    {
        index = first index of left array
    }
    if (value at right array first index is equal to index)
    {
        index = first index of right array
    }

}







*/