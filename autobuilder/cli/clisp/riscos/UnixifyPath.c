#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <swis.h>
#include <unixlib/local.h>

enum
{
 NAME_BUFFER_SIZE = 258,
 VAR_BUFFER_SIZE = 1024
};

int main (int argc, char* argv[]) {
 char nameBuffer[NAME_BUFFER_SIZE] = "U_";
 const char *inName;
 unsigned int inNameLen;

 char varBuffer[VAR_BUFFER_SIZE];
 unsigned int varLen;
 char *unixifiedPath = NULL;

 if (argc != 2)
 {
  fprintf(stderr, "UnixifyPath takes exactly 1 argument (the name of the system variable containing the path).");
  exit(1);
 }
 inName = argv[1];
 inNameLen = strlen(inName);

 if (inNameLen+2 > NAME_BUFFER_SIZE-1)
 {
  fprintf(stderr, "Name of system variable containing path to unixify is unexpectedly long.");
  exit(1);
 }
 strcat(nameBuffer, inName);

 _swi( OS_ReadVarVal, _IN(0) | _IN(1) | _IN(2) | _IN(3) | _IN(4) | _OUT(2),
   inName, varBuffer, VAR_BUFFER_SIZE-1, 0, 0, &varLen );
 varBuffer[varLen] = 0;

 unixifiedPath = __unixify(varBuffer, 0, NULL, 0, __RISCOSIFY_FILETYPE_NOTSPECIFIED);

 if (!unixifiedPath)
 {
  fprintf(stderr, "Unixify call failed.");
  exit(1);
 }

 _swi( OS_SetVarVal, _IN(0) | _IN(1) | _IN(2) | _IN(3) | _IN(4),
   nameBuffer, unixifiedPath, strlen(unixifiedPath), 0, 4 );

 free( unixifiedPath );
 exit(0);
}
