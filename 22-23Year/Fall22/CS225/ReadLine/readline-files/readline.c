#include "readline.h"
#include <stdlib.h>
#include <string.h>


char *readline(FILE *file)
{
    char *buffer = calloc(1, 2);
    int capacity = 2;
    int size = 0;

    if (file != NULL)
    {

        while (fgets(buffer + size, capacity - size, file) != NULL)
        {
            size = strlen(buffer);
            
            
            if (buffer[size - 1] == '\n')
            {
                buffer[size - 1] = '\0';
                break;
            }
            /*
            printf("Capacity: %i\n", capacity);
            printf("Size: %i\n", size);
            printf("Buffer: %s\n", buffer);
            printf("---------------------------\n");
            */
            if (size + 1 == capacity)
            {
                char *newbuffer = realloc(buffer, capacity * 2);
                if (newbuffer != NULL)
                {
                    buffer = newbuffer;
                }
                capacity *= 2;
            }

            
        }
    }

    return buffer;
}

