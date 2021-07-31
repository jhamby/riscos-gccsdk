/* minimal ls, printf space separated filenames */
/* -l option, printf newline separated filenames */
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int lscmd(int argc, char *argv[])
{
	DIR 		   *p_dir;
	struct dirent  *p_dirent;
	char 		   *file_name;
	int sep=32;
	if (argc < 2)
	argv[1] = "@";
	if (!strcmp(argv[1],"-l")) {
	  sep=10;
	  if (argc < 3)
	  argv[1] = "@";
	  else
	  argv[1]=argv[2];
	 }
	if ((p_dir = opendir(argv[1])) == NULL) {
		printf("opendir(%s) failed\n", argv[1]);
		printf("Useage ls [-l] [directory] or default to current\n");
		return(1);
	}
	p_dir = opendir(argv[1]);
 p_dirent = readdir(p_dir); /* skip . and .. */
 p_dirent = readdir(p_dir);
	while ((p_dirent = readdir(p_dir)) != NULL) {
		 file_name = p_dirent->d_name;
			printf("%s%c",file_name,sep);
		}
	 closedir(p_dir);
	 if (sep==32)
	 printf("\n");
	return 0;
}
