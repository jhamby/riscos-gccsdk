/* Factorial printer range (1-2147483647) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc,char *argv[])
{
   int a,b,c,d,f,q,s,x;
   float len,digits;
   char *t;

   if ( argc==1 ) {
      printf("Usage: Factorial <number>\n");
      return 0;
   }
   f=atoi(argv[1]);
   if (f<=1 || f>2147483647) {
      printf("I can only factorialise numbers between 2 and 2147483647\n");
      return 0;
   }

   len=(float)1.0;
   for(q=2;q<=f;q++)
      len+=(float)log10(q);
   t=(char *)calloc( (int)len,sizeof(char));

   for(q=1;q<=len;q++)
      t[q]=0;

   t[1]=1;
   digits=(float)1.0;
   c=0;
   for(a=2;a<=f;a++) {
      digits+=(float)log10(a);
      for(b=1;b<=digits;b++) {
         s=t[b]*a+c;
         t[b]=s % 10;
         c=s / 10;
      }
   }
   printf("%d ! = \n\n",f);
   for(d=(int)len;d>=1;d--)
      printf("%d",t[d]);
   x=(int)len;
   printf("\n\nwhich is approx. %d x 10^%d\n",t[x],x-1);
   printf("Time: %.2f seconds\n",clock() / 100.0);
   return 0;
}
