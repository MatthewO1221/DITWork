/*
Author: Matthew Oliver
File: fuse.c
Purpose: fuse.c can either take any number of text files and turn them into one .fused file, or it can take
a .fused file and convert it back into text files
Copyright: 

*/


#include "fuse.h"
#include <stdio.h>  /* fopen/fclose/fread/fwrite */
#include <stdlib.h> /* malloc/free */
#include <string.h> /* strlen */

#define xDEBUG

/*
Function: fuse
Returns:    int - Whether the function was successful or not
            output - The new fused file
Inputs:     filenames - A list of filenames to be fused
            num_files - The number of files to be fused
Purpose: fuse takes a list of files and "fuses" them together into one formatted file
*/
int fuse(char const **filenames, int num_files, char const *output)
{
  int BUFFER_SIZE = 1 << 16; /* 65 Kb */
  char *content; /*Buffer for current file's content to be read into*/
  FILE *outputFile;
  int size;
  outputFile = fopen(output, "wb"); /*Opening fused output file*/
  if (outputFile)
  {
    int i;
    for (i = 0; i < num_files; i++) /*Loops through all the given files*/
    {

      FILE *stream = fopen(filenames[i], "rb"); /*Open the current file to be read*/

      if (stream)
      {

        content = malloc(BUFFER_SIZE); /*malloc content, BUFFER_SIZE is an assumed value*/
        
        /*Two purposes: 1. Returns an int of the number of bytes read. 
                        2. Reads the contents of the current file into the buffer*/
        size = fread(content, sizeof(char), BUFFER_SIZE, stream); 

        /*Four writes, first the filename, then a null character, 
        then the size of that file's content, then finally the content itself*/
        fwrite(filenames[i], sizeof(char), strlen(filenames[i]), outputFile);
        fwrite("\0", sizeof(char), 1, outputFile);
        fwrite(&size, sizeof(int), 1, outputFile);
        fwrite(content, sizeof(char), size, outputFile);

        /*Free the content buffer and close the current file*/
        free(content);
        fclose(stream);
      }
      else if (!stream)
      {
        return E_BAD_SOURCE; /*If the current file couldn't open*/
      }
      
    }

    fclose(outputFile);
  }
  else if (!outputFile)
  {
    return E_BAD_DESTINATION; /*If the ouput file couldn't open*/
  }
  

  return 0;
}

/*
Function: unfuse
Returns:    int - Whether the function was successful or not
            output - Creates new files from fused file
Inputs:     filename - The name of a fused file to be unfused
Purpose: unfuse takes a fused file and undoes the process, turning it back into text files
*/
int unfuse(char const *filename)
{
  FILE *stream = fopen(filename, "rb"); /*Open the fused file*/
  FILE *outputFile; /*Pointer for file to output to*/
  char *buffer; /*Buffer for data to be read*/
  char *curFileName = calloc(1, 256); /*Calloc an empty string for the filenames*/
  int bytesRead = 0; /*Int to keep track of where in the fused file the function is*/
  if (stream && curFileName)
  {

    while (!feof(stream)) /*Repeat until the end of the file is reached*/
    {
      if (bytesRead == 0) /*Do this in the first loop*/
      {
        fread(curFileName, sizeof(char), 256, stream); /*Read the name of the current file to make*/
      }
      
      bytesRead += strlen(curFileName) + 1; /*Increment bytesRead by the length of the file name + the null character*/
      
      fseek(stream, bytesRead, SEEK_SET); /*Seek back to the correct position in the file*/
      /*printf("%s\n", curFileName);*/
       outputFile = fopen(curFileName, "wb"); /*Open the output file using the current filename*/

      if (outputFile)
      {
        int size = 0; /*The size of the contents of the current file*/

        fread(&size, sizeof(int), 1, stream); /*Read the size of the contents and put it in size*/

        /*Increment bytesRead by the size of the content and the size integer itself*/
        bytesRead += size + sizeof(int); 
        /*printf("%i\n", size);*/

        
        buffer = malloc(size); /*malloc a buffer for the file content*/
        if (buffer)
        {
          fread(buffer, sizeof(char), size, stream); /*Read the contents of the current file*/
          /*printf("%s\n", buffer);*/
          fwrite(buffer, sizeof(char), size, outputFile); /*Write the contents to the ouput file*/
          free(buffer);
        }

        fclose(outputFile);
      }
      else if (!outputFile)
      {
        /*If the output file couldn't open*/
        return E_BAD_DESTINATION;
      }

      fread(curFileName, sizeof(char), 256, stream); /*fread at the end so the end of file flag is set correctly*/
    }

    fclose(stream);
  }
  else if(!stream)
  {
    return E_BAD_SOURCE; /*If the input file couldn't open*/
  }

  free(curFileName);
  return 0;
}
