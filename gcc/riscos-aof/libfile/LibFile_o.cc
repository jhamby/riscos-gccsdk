#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>

#ifndef UNIX
extern "C" OS_File(int *);
#endif

void writeInt(FILE *a_out,int a_data);

//----------------------------------------------------------

class TimeStamp
{
 public:
 	TimeStamp(int a_load,int a_exe)
 	{
 	 m_load=a_load;
 	 m_exe=a_exe;
 	}

 	put(FILE *a_out)
 	{
 	 int p=((m_load&255)<<16)|((m_exe>>16)&0xFFFF);
 	 writeInt(a_out,p);
 	 p=((m_exe&0xFF)<<24)|((m_exe&0xFF00)<<8);
 	 writeInt(a_out,p);
 	}

 	int m_load,m_exe;
};

//----------------------------------------------------------

int offset=0;

//----------------------------------------------------------

void usage()
{
 cout << "AOF Librarian" << endl;
 cout << "Version 1.00 compiled by gcc 2.7.2.1" << endl;
 cout << "ALF creation and maintenance tool" << endl;
 cout << "(02-Jul-1997) by Bernhard Walter" << endl;
 cout << endl;
 cout << "Syntax: LibFile <Options> <Library> [ <FileList> | <MemberList> ]" << endl;
 cout << endl;
 cout << "Options:" << endl << endl;
 cout << "-c        Create new library containing files in <FileList>" << endl;
 cout << "-i        Insert files in <FileList>, replace existing members of same name" << endl;
 cout << "-d        Delete the members in <MemberList>" << endl;
 cout << "-e        Extract members in <MemberList>, place them in files of same name" << endl;
 cout << "-o        Add external symbol table to object library (for use with linker)" << endl;
 cout << "-l        List library (combine with any other option)" << endl;
 cout << "-s        List symbol table (combine with any other option" << endl;
 cout << "-v <File> Take further arguments from file <File>" << endl;
 cout << "-h        Displays help" << endl;
 cout << "-t        Use NULL timestamps" << endl;
 cout << "-q <Dir>  Place relative filenames under directory <Dir>" << endl;
 cout << endl;
}

//----------------------------------------------------------

int exists(char *a_file)
{
 FILE *fp=fopen(a_file,"r");
 if(!a_file)
  return 0;
 fclose(fp);
 return(1);
}

//----------------------------------------------------------

TimeStamp readTimeStamp(char *a_fileName)
{
#ifdef UNIX
 return TimeStamp (0, 0);
#else
 int reg[6]={17,0,0,0,0,0};

 reg[1]=(int) a_fileName;
 OS_File(reg);
 return(TimeStamp(reg[2],reg[3]));
#endif
}

//----------------------------------------------------------

static int readFileSize(char *a_fileName)
{
#ifdef UNIX
 struct stat f;

 if (stat (a_fileName, &f) == -1)
   return -1;

 printf ("readFileSize: %s, size = %d\n", a_fileName, f.st_size);
 return f.st_size;
#else
 int reg[6]={17,0,0,0,0,0};

 reg[1]=(int) a_fileName;
 OS_File(reg);
 if(reg[0]!=1)
 	return(-1);
 return(reg[4]);
#endif
}

//----------------------------------------------------------

void fputcCounted(int a_data,FILE *a_out)
{
 fputc(a_data,a_out);
 offset++;
}

//----------------------------------------------------------

int readInt(FILE *a_out)
{
 int i,r=0;
 for(i=0;i<4;i++)
 	r+=(fgetc(a_out)&255)<<(i*8);
 return r;
}

//----------------------------------------------------------

void writeInt(FILE *a_out,int a_data)
{
 fputcCounted(a_data&255,a_out);
 fputcCounted((a_data>>8)&255,a_out);
 fputcCounted((a_data>>16)&255,a_out);
 fputcCounted((a_data>>24)&255,a_out);
}

//----------------------------------------------------------

void writeChunkHeader(FILE *a_out,char *a_name,int a_offset,int a_len)
{
 fprintf(a_out,"%s",a_name);
 writeInt(a_out,a_offset);
 writeInt(a_out,a_len);
}

//----------------------------------------------------------

int calcDirSize(int argc,char **argv)
{
 int i,l,len=0;
 for(i=2;i<argc;i++)
 {
  if(exists(argv[i]))
  {
   l=strlen(argv[i])+1;
   while(l&3) l++;
   len+=4+4+4+l+8;
  }
 }
 return len;
}

//----------------------------------------------------------

void writeLibDir(FILE *a_out,int argc,char **argv)
{
 int i,k,l,len;
 for(i=2;i<argc;i++)
 {
  if(exists(argv[i]))
  {
   l=strlen(argv[i])+1;
   while(l&3) l++;
   len=4+4+4+l+8;
   writeInt(a_out,i+1); // Chunk Index
   writeInt(a_out,len); // Entry Length
   writeInt(a_out,l+8); // Data Length
   fprintf(a_out,"%s",argv[i]);		// Data
   for(k=strlen(argv[i]);k<l;k++)	// Padding
   	fputc(0,a_out);
   readTimeStamp(argv[i]).put(a_out);	// Time-Stamp
  }
 }
}

//----------------------------------------------------------

FILE *findChunk(char *file,char *name,int *start,int *len)
{
 int i,j,chunks;
 char bf[9];
 FILE *inf;

 *start=-1;
 *len=-1;

 inf=fopen(file,"rb");

 int id=readInt(inf);
 if(id!=0xC3CBC6C5)
 {
  fclose(inf);
  return NULL;
 }

 readInt(inf);
 chunks=readInt(inf);
 for(j=0;j<chunks;j++)
 {
  for(i=0;i<8;i++)
   bf[i]=fgetc(inf);
  bf[8]=0;
  if(!strcmp(bf,name))
  {
   *start=readInt(inf);
   *len=readInt(inf);
  }
  else
  {
   readInt(inf);
   readInt(inf);
  }
 }

 fclose(inf);
 if((*start)==-1)
  return NULL;

 inf=fopen(file,"rb");
 for(j=0;j<(*start);j++)
  fgetc(inf);

 return inf;
}

//----------------------------------------------------------

int cmpf(const void *e1,const void *e2)
{
 int i1=*((int *) e1);
 int i2=*((int *) e2);
 return(i1-i2);
}

//----------------------------------------------------------

char **getSymt(char *file,int *tlen)
{
 int j,i,k,start=0,len=0,chunks;
 char bf[9],**res=NULL;
 FILE *inf;

 *tlen=0;

 inf=findChunk(file,"OBJ_SYMT",&start,&len);
 int syms=len>>4;
 int *idx=NULL;
 int externs=0;
 for(i=0;i<syms;i++)
 {
  j=readInt(inf);
  k=readInt(inf);
  if(k&1)	// external Symbol
  {
   externs++;
   idx=(int *) realloc(idx,externs*sizeof(int));
   idx[externs-1]=j;
  }
  readInt(inf);
  readInt(inf);
 }
 fclose(inf);
 res=(char **) malloc(externs*sizeof(char*));
 qsort(idx,externs,sizeof(int),cmpf);
 inf=findChunk(file,"OBJ_STRT",&start,&len);
 k=readInt(inf);
 int pos=4,ch,slen;
 char sbf[1000];
 for(i=0;i<externs;i++)
 {
  for(;pos<idx[i];pos++)
   fgetc(inf);
  slen=0;
  while((ch=fgetc(inf))!=0)
  {
   pos++;
   sbf[slen]=ch;
   slen++;
  }
  sbf[slen]=0;
  pos++;
  res[i]=(char *) malloc(slen+1);
  strcpy(res[i],sbf);
 }
 *tlen=externs;
 fclose(inf);
 free(idx);
 return res;
}

//----------------------------------------------------------

int writeSymbolTable(FILE *a_out,int argc,char **argv,int doWrite=1)
{
 int k,len,sl,i,j;
 char **symt;
 int bytes=0;

 for(i=2;i<argc;i++)
 {
  if(exists(argv[i]))
  {
   symt=getSymt(argv[i],&len);
   for(k=0;k<len;k++)
   {
    sl=strlen(symt[k])+1;
    while(sl&3) sl++;
    if(doWrite)
    {
     writeInt(a_out,i+1); 	// Chunk Index
     writeInt(a_out,sl+4+4+4);	// Entry Length
     writeInt(a_out,sl); 	// Data Length
     fprintf(a_out,"%s",symt[k]);	// Data
     for(j=strlen(symt[k]);j<sl;j++)	// Padding
   	fputc(0,a_out);
    }
    else
     bytes+=4+4+4+sl;		// Entry length
   }
   for(k=0;k<len;k++)
    free(symt[k]);
   free(symt);
  }
 }
 return bytes;
}

//----------------------------------------------------------

void writeLibData(FILE *a_out,int argc,char **argv)
{
 int i,ch;
 FILE *inf;

 for(i=2;i<argc;i++)
 {
  inf=fopen(argv[i],"rb");
  if(inf)
  {
   while((ch=fgetc(inf))!=EOF)
   	fputc(ch,a_out);
   fclose(inf);
  }
 }
}

//----------------------------------------------------------

void writeHeader(FILE *a_out,int a_chunks)
{
 // ChunkFileId
 writeInt(a_out,0xC3CBC6C5);

 // maxChunks
 writeInt(a_out,a_chunks);

 // numChunks
 writeInt(a_out,a_chunks);
}

//----------------------------------------------------------

int main(int argc,char **argv)
{

 if(argc<3)
 {
  	usage();
  	exit(1);
 }

 FILE *out;

 out=fopen(argv[1],"wb");
 fclose(out);
 TimeStamp now=readTimeStamp(argv[1]);

 out=fopen(argv[1],"wb");
 if(!out)
 {
  	fprintf(stderr,"File '%s' could not be opened\n",argv[1]);
  	exit(1);
 }

 int dirSize=calcDirSize(argc,argv);

 int dataPtr=0;
 dataPtr+=3*4;		// File Header
 dataPtr+=16;		// Lib Time
 dataPtr+=16;		// Lib Version
 dataPtr+=16;		// Lib Dir
 dataPtr+=16*(argc-2);	// Obj Files
 dataPtr+=16;		// OFL-Time
 dataPtr+=16;		// OFL_SYMT
 // TODO: OFL_SYMT

 int i,dp=dataPtr,dl;

 writeHeader(out,argc-2+6);
 writeChunkHeader(out,"LIB_TIME",dp,8);dp+=8;
 writeChunkHeader(out,"LIB_VRSN",dp,4);dp+=4;
 writeChunkHeader(out,"LIB_DIRY",dp,dirSize);dp+=dirSize;

 for(i=2;i<argc;i++)
 {
  dl=readFileSize(argv[i]);
  while(dl&3) dl++;
  writeChunkHeader(out,"LIB_DATA",dp,dl);
  dp+=dl;
 }

 writeChunkHeader(out,"OFL_TIME",dp,8);dp+=8;
 int symtSize=writeSymbolTable(out,argc,argv,0);
 writeChunkHeader(out,"OFL_SYMT",dp,symtSize);

 now.put(out);
 writeInt(out,1);
 writeLibDir(out,argc,argv);
 writeLibData(out,argc,argv);
 now.put(out);
 writeSymbolTable(out,argc,argv);

 fclose(out);
}
