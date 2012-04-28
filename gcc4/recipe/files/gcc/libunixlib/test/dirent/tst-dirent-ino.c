#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

/* Tests the inode consistency.  */
int
main (void) 
{
   struct stat dot_sb;
   if (stat (".", &dot_sb) < 0)
     return EXIT_FAILURE;

   DIR *dirp = opendir ("..");
   struct dirent const *dp;
   int cnt = 0;
   while ((dp = readdir (dirp)) != NULL)
     {
       if (dp->d_ino == dot_sb.st_ino)
         ++cnt;
     }
   /* cnt should always be 1 *except* when our CWD is the root directory.
      In that case, cnt is 2 (as opendir ("..") gives the same enumeration
      as opendir (".")).  */
   assert (cnt == 1 || cnt == 2);

   return EXIT_SUCCESS;
}
