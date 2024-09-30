#include <stdio.h>    // printf, perror
#include <string.h>   // strcpy, strlen, memcpy
#include <stdlib.h>   // malloc, free
#include <fcntl.h>    // open, O_WRONLY, O_CREAT, O_TRUNC
#include <sys/stat.h> // S_IRUSR, S_IWUSR
#include <unistd.h>   // close, write, fsync
#include "logger.h"   // logger stuff

static unsigned char *LOG_BUFFER;  // The memory buffer
static unsigned int LOG_BUFSIZE;   // The size of the buffer
static unsigned int LOG_BYTECOUNT; // How many bytes in the buffer?
static char *LOG_FILENAME;         // The name of the file to write the log to
static int LOG_FH = -1;            // The file handle of the open file

LOG_RESULT log_open(const char *filename, unsigned int buffersize)
{
  LOG_FILENAME = filename;
  LOG_FH = open(LOG_FILENAME, O_WRONLY, O_CREAT);

  if (LOG_FH == -1)
  {
    return LOG_OPENFAIL;
  }

  LOG_BUFFER = malloc(buffersize);

  if (!LOG_BUFFER)
  {
    return LOG_NOMEM;
  }

  return LOG_SUCCESS;
}

LOG_RESULT log_write(const unsigned char *text)
{
  if (LOG_BUFFER)
  {

    while (*text != '\0')
    {
      *LOG_BUFFER = *text;
      

      if (LOG_BYTECOUNT == LOG_BUFSIZE)
      {
        LOG_RESULT writeCheck = log_flush();

        if (writeCheck == LOG_WRITEFAIL)
        {
          return LOG_WRITEFAIL;
        }
      }
      text++;
      LOG_BUFFER++;
      LOG_BYTECOUNT++;
    }
  }

  return LOG_SUCCESS;
}

LOG_RESULT log_flush(void)
{
  if (LOG_FH != -1 && LOG_BUFFER)
  {
    int result = write(LOG_FH, LOG_BUFFER, LOG_BYTECOUNT);
    fsync(LOG_FH);

    if (result == -1)
    {
      return LOG_WRITEFAIL;
    }

    LOG_BUFFER = "";
    LOG_BUFFER -= LOG_BUFSIZE;
    LOG_BYTECOUNT = 0;
  }

  return LOG_SUCCESS;
}

LOG_RESULT log_close(void)
{
  if (log_flush() == LOG_WRITEFAIL)
  {
    return LOG_WRITEFAIL;
  }

  if (LOG_FH != -1)
  {
    close(LOG_FH);
  }
  LOG_BYTECOUNT = 0;
  LOG_BUFSIZE = 0;
  LOG_FH = -1;

  free(LOG_BUFFER);
  free(LOG_FILENAME;)
  
}

// Doxygen function header goes here.
void log_dump(void)
{
  printf("BUFF[");
  for (unsigned i = 0; i < LOG_BYTECOUNT; i++)
    printf("%c", LOG_BUFFER[i]);
  printf("]BUFF\n");
}