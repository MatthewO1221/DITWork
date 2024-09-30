#include "replace.h"
// no more #includes

int *FindReplaceables(char *str, int str_size, char const *find_substr, int find_size, int buffer_size);

std::tuple<bool, int> replace(char const *find_substr, int find_size,
                              char const *repl_substr, int repl_size,
                              char *str, int str_size,
                              int buffer_size)
{
  // rCount keeps track of how many characters in str have matched
  // find_substr in a row
  int rCount = 0;

  // Iterates through str
  for (int i = 0; i < buffer_size; i++)
  {
    // Compare the current character in str to a character
    // in find_substr
    if (str[i] == find_substr[rCount])
    {
      // If they're the same increment rCount
      rCount++;
    }
    else
      rCount = 0;
    // If they aren't the same reset rCount

    // If rCount is ever = to find size then we've found
    // an instance of find_substr
    if (rCount == find_size)
    {
      // If the find_substr and repl_substr are the same length
      // Then we don't need to do any shifting
      if (find_size == repl_size)
      {
        // A for loop that goes for repl_size iterations
        for (int j = 0; j < repl_size; j++)
        {
          // Replace the characters in str with the characters in
          // repl_substr
          str[i - find_size + j + 1] = repl_substr[j];
        }
        // Reset rCount
        rCount = 0;
      }
      // If the find_substr is longer than the repl_substr
      // Then we need to shift everything after the replace left
      else if (find_size > repl_size)
      {

        
          // Iterate through str after the last character that will be replaced
          for (int k = (i - find_size) + repl_size + 1; k < str_size; k++)
          {
            // If we try to go outside of memory
            if (k + (find_size - repl_size) >= buffer_size)
            {
              // Just set it to null
              str[k] = '\0';
            }
            else
            {
              // Everything in str is = to the next thing is str
              str[k] = str[k + (find_size - repl_size)];
            }
          }

        // Do the actual replacing
        for (int j = 0; j < repl_size; j++)
        {
          str[i - find_size + j + 1] = repl_substr[j];
        }

        str_size -= find_size - repl_size;
        rCount = 0;
      }
      // If the find_substr is shorter than the repl_substr
      // Then we need to shift everything after the replace right
      else if (find_size < repl_size)
      {
        // If we would overwrite the buffer then return false
        if (str_size + (repl_size - find_size) > buffer_size)
        {
          return std::tuple<bool, int>{false, 0};
        }

          // Iterate from end of list to character just after last find
          for (int k = str_size + 1; k > i; k--)
          {
            // Shift everything right
            str[k] = str[k - (repl_size - find_size)];
          }
        

        // Do the actual replacing
        for (int j = 0; j < repl_size; j++)
        {
          str[i - find_size + j + 1] = repl_substr[j];
        }

        // Increment str_size by the difference
        str_size += repl_size - find_size;

        // Reset rCount
        rCount = 0;
      }
    }
  }

  // Should always return true unless we tried to go outside the buffer
  return std::tuple<bool, int>{true, str_size};
}