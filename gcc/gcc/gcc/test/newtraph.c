/* Timing of a C program composed mainly of real arithmetic */
/* P.S.Williams */

#include <stdio.h>
#include <time.h>

int main(void)

{

 double x1,x2,x3,j11,j12,j13,j21,j22,j23,j31,j32,j33 ;
 double f1,f2,f3,i11,i12,i13,i21,i22,i23,i31,i32,i33 ;
 double newX1,newX2,newX3,c1,c2,c3,det ;
 int t1,t2,t;
 int count,maxNoIts;



 printf(" Generalised Newton Raphson in C \n");

 t1 = clock();

 maxNoIts = 10000;

 x1 = 0.25; x2 = 0.5; x3 = 0.75;

 c1 = (x1+x2+x3)*(x1+x2+x3);
 c2 = (x1+2*x2+3*x3)*(x1+2*x2+3*x3);
 c3 = (2*x1-x2-x3)*(2*x1-x2-x3);

 x1 = 0.2; x2 = 0.45; x3 = 0.7;
  
 for(count = 0; count <= maxNoIts; count++)
  {

      f1 = (x1+x2+x3)*(x1+x2+x3)         - c1;
      f2 = (x1+2*x2+3*x3)*(x1+2*x2+3*x3) - c2;
      f3 = (2*x1-x2-x3)*(2*x1-x2-x3)     - c3;

      j11 = 2*(x1+x2+x3);     j12 = 2*(x1+x2+x3);     j13 = 2*(x1+x2+x3);
      j21 = 2*(x1+2*x2+3*x3); j22 = 4*(x1+2*x2+3*x3); j23 = 6*(x1+2*x2+3*x3);
      j31 = 4*(2*x1-x2-x3);   j32 = -(2*x1-x2-x3);    j33 = -(2*x1-x2-x3);



      det =  j11*j22*j33 - j11*j23*j32 
           - j12*j21*j33 + j12*j23*j31
           + j13*j21*j32 - j13*j22*j31;


      i11 =  (j22*j33 - j23*j32)/det;
      i12 = -(j12*j33 - j13*j32)/det;   
      i13 =  (j12*j23 - j13*j22)/det;  
                                                                               
      i21 = -(j21*j33 - j23*j31)/det;
      i22 =  (j11*j33 - j13*j31)/det;
      i23 = -(j11*j23 - j13*j21)/det;
                                                                               
      i31 =  (j21*j32 - j22*j31)/det;
      i32 = -(j11*j32 - j12*j31)/det;
      i33 =  (j11*j22 - j12*j21)/det;

      newX1 = x1 - (i11*f1+i12*f2+i13*f3);
      newX2 = x2 - (i21*f1+i22*f2+i23*f3);  
      newX3 = x3 - (i31*f1+i32*f2+i33*f3);

      x1 = newX1; x2 = newX2; x3 = newX3;
                         
     
 }

 t2 = clock();
 t = t2-t1;
 printf("%d iterations took %d centi-secs\n",maxNoIts,t);
 printf("x1 = %e x2 = %e x3 = %e",x1,x2,x3);

 return(0);


}
