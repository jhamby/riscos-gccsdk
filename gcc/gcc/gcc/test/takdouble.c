/* tak program */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

double tak(double,double,double);

int main(int argc, char *argv[])
{
   int a,b,c;

   if ( argc==1 || argc==2 || argc==3 ) return 0;
   a=atoi(argv[1]);
   b=atoi(argv[2]);
   c=atoi(argv[3]);
   printf("TAK(%d,%d,%d) = %d\n",a,b,c,(int)tak(a,b,c));
   printf("Time = %d\n",clock() );
   return 0;
}

double tak(double x,double y,double z)
{
   if (y>=x) return (z);
   else
     return ( tak( tak(x-1,y,z), tak(y-1,z,x), tak(z-1,x,y) ) );
}
