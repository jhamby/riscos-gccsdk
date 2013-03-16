/*
    Simple Program to create a RISC OS wrapper for a downloaded AlephOne game

    Copyright (C) 2008  Alan Buckley

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    A full version of the license can be found in the COPYING file
    included with this program.
    
    Contact the Author at <alan_baa@hotmail.com>
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <swis.h>

void usage();
void create_run(char *title, char *dest, char *appname);
void create_help(char *title, char *dest);
void create_sprites(char *dest, char *appname);

void set_filetype(char *filename, int type);

/* Main entry point */

int main(int argc, char *argv[])
{
    char *title, *dest, *appname;

    if (argc < 2)
    {
        usage();
        return 0;
    }

    title = argv[1];

    if (!*title)
    {
       printf("Error: Empty title\n");
       return -1;
    }

    dest = argv[2];
    appname = strrchr(dest, '!');
    if (appname == 0)
    {
       printf("Error: Application name is missing\n");
       return -2;
    }
    appname++;

    if (*appname == 0 || strlen(appname) > 9)
    {
       printf("Error: Application name must be from 1 to 9 characters long\n");
       return -3;
    }
   
    if (mkdir(dest) != 0)
    {
       printf("Error: Unable to create the target directory\n");
       return -4;
    }

    // Create the data dir
    {
      char datadir[256];
      strcpy(datadir,dest);
      strcat(datadir, ".data");
      if (mkdir(datadir) != 0)
      {
         printf("Error: Unable to create the data directory\n");
         return -5;
      }
    }
    create_run(title, dest, appname);
    create_help(title, dest);
    create_sprites(dest, appname);

    return 0;
}

void usage()
{
   printf("Create a RISC OS wrapper application for an AlephOne SDL game\n");
   printf("Once the wrapper is created the game data should be unzipped into\n");
   printf("the data directory inside the wrapper\n\n");

   printf("aowrapper <game_name> <dest_app>\n");
   printf("   <game_name> is the name of the game (in quotes if it include spaces\n");
   printf("   <dest_app> is the name of the wrapper application to create\n");

   printf("\nExample:\n");
   printf("aowrapper \"Marathon 1\" ^.!M1A1\n");
}

/**
 * Create the run file
 */
void create_run(char *title, char *dest, char *appname)
{
    char filename[256];

    strcpy(filename, dest);
    strcat(filename, ".");
    strcat(filename, "!Run");

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
       printf("Error:Unable to create the run file: %s\n", filename);
       return;
    }

    fprintf(fp, "| Run file %s\n", title);
    fputs("\n", fp);
    fputs("Wimpslot -min 12768K -max 12768K", fp);
    fputs("\n", fp);
    fputs("If \"<AlephOne$Dir>\" = \"\" Then Error !AlephOne must have been seen by the filer before running this application", fp);
    fputs("\n", fp);
    fputs("| Set up data directories for !AlephOne engine\n", fp);

    fprintf(fp, "Set %s$Path <Obey$Dir>.data.\n", appname);
    fprintf(fp, "Set ALEPHONE_DATA AlephOne:;%s:\n", appname);

    fputs("\n", fp);
    fputs("| Set the title for the map", fp);
    fputs("\n", fp);
    fprintf(fp, "Set SDL$AlephOne$TaskName \"%s\"\n\n", title);
    fputs("Run <AlephOne$Dir>.alephone >null: 2>null:", fp);

    fclose(fp);

    set_filetype(filename, 0xfeb); // Obey
}

/**
 * Create a skeleton help directory
 */
void create_help(char *title, char *dest)
{
    char filename[256];

    strcpy(filename, dest);
    strcat(filename, ".");
    strcat(filename, "!Help");

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
       printf("Error:Unable to create the Help file: %s\n", filename);
       return;
    }

    fputs(title, fp);
    fputs(" (Aleph One SDL version)\n",fp);
    fputs("\n", fp);

    fputs("The !AlephOne application directory must have been seen before)\n",fp);
    fputs("this application is run.\n",fp);
    fputs("\n", fp);

    fputs("See the !AlephOne help file for details on Aleph One including the\n",fp);
    fputs("keys required to operate the game.\n",fp);
    fputs("\n", fp);

    fputs("The aowrapper program created this RISC OS wrapper\n", fp);
    fputs("The data for the game should have been copied into\n", fp);
    fputs("the data subdirectory\n", fp);
    fputs("\n", fp);
    fputs("You may want to replace this help with the help for the\n", fp);
    fputs("game you are wrapping. This can often be found in a README\n", fp);
    fputs("in the game archive\n", fp);
    fclose(fp);
}

void create_sprites(char *dest, char *appname)
{
    char filename[256];
    char *spritedata;
    int spritelen;
    char *p;

    // Use AlephOne sprite
    strcpy(filename, "<AlephOne$Dir>.!Sprites22");
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
       printf("Error: Unable to read the AlephOne Sprite file: %s\n", filename);
       return;
    }

    fseek(fp, 0, SEEK_END);
    spritelen = ftell(fp);
    fseek(fp, 0, SEEK_SET);    
    if (spritelen < 64)
    {
       fclose(fp);
       printf("Error: AlephOne sprite file is too small\n");
       return;
    }

    spritedata = malloc(spritelen);
    if (!spritedata)
    {
       fclose(fp);
       printf("Error: Unable to allocate memory to copy sprite\n");
       return;
    }

    fread(spritedata, 1, spritelen, fp);
    fclose(fp);       

    /* Build copy - with name changed */
    p = spritedata + 16;
    *p++='!';
    while (*appname)
    {
       *p++ = tolower(*appname);
       appname++;
    }
    *p = 0;

    strcpy(filename, dest);
    strcat(filename, ".");
    strcat(filename, "!Sprites");

    fp = fopen(filename, "wb");
    if (fp == NULL)
    {
       printf("Error:Unable to create the Sprites file copy: %s\n", filename);
    } else
    {
       fwrite(spritedata, 1, spritelen, fp);
       fclose(fp);

       set_filetype(filename, 0xff9);
    }
}

/**
 * Set the RISC OS File type
 */
void set_filetype(char *filename, int type)
{
   _swix(OS_File, _INR(0,2), 18, (int)filename, type);
}
