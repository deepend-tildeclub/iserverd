#include "config.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_TIME_H
#include <time.h>
#endif

int main(int argc, char **argv)
{
   char tempst[1024];
   int fd = -1;

   if (argc < 2)
   {
      printf("usage: %s filenameXXXXXX\n", argv[0]);
      exit(-1);
   }

   strncpy(tempst, argv[1], sizeof(tempst) - 1);
   tempst[sizeof(tempst) - 1] = '\0';

   if (strstr(tempst, "XXXXXX") == NULL)
   {
      fprintf(stderr, "error: template must include XXXXXX\n");
      return(1);
   }

#ifdef HAVE_MKSTEMP
   fd = mkstemp(tempst);
   if (fd < 0)
   {
      perror("mkstemp");
      return(1);
   }
#else
   char *xst = strstr(tempst, "XXXXXX");
   const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
   int i;

   srand((unsigned int)(time(NULL) ^ getpid()));
   for (i = 0; i < 256; i++)
   {
      int j;

      for (j = 0; j < 6; j++)
      {
         xst[j] = letters[rand() % (sizeof(letters) - 1)];
      }

      fd = open(tempst, O_CREAT|O_EXCL|O_RDWR, 0600);
      if (fd >= 0)
      {
         break;
      }

      if (errno != EEXIST && errno != EINTR)
      {
         perror("open");
         return(1);
      }
   }

   if (fd < 0)
   {
      fprintf(stderr, "error: unable to create temporary file securely\n");
      return(1);
   }
#endif

   if (fd >= 0)
   {
      close(fd);
   }

   printf("%s\n", tempst);
   return(0);
}
