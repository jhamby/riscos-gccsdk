/* Rich Walker.  */

/* I'm not _certain_ if there isn't something I can do to eliminate this
occurrence, but on the basis that I can't see why it's happening, I'm
reporting it as a bug.

Given the macro definition

#define F_mla_wide_noi(a,b,c) asm("SMLAL %0,%1,%2,%3" \
	: "=&r" (c.lo), "=&r" (c.hi) \
	: "%r" (a), "%r" (b), "0" (c.lo), "1" (c.hi) )

and the types 

typedef signed long Fixed;
typedef struct {     unsigned long lo;  signed long hi; } FixedD;

the fragment

    Fixed *ip;
    register FixedD s;
    ip = input; 
    s=FD_0;
    for (j=0; j<l->prev_size; j++)
    {
      Fixed a,b;
      a=*wp++;
      b=*ip++;
      F_mla_wide_noi(a,b,s);
    };

compiles into 
|L..103|
	ldr	a4, [v3], #4
	ldr	a1, [lr], #4
	mov	v4, a3
	SMLAL a2,v4,a4,a1
	mov	a4, v4
	mov	a3, a4
	add	ip, ip, #1
	ldr	v2, [sp, #4]
	cmp	ip, v2
	blt	|L..103|

rather than 
|L..103|
	ldr	a4, [v3], #4
	ldr	a1, [lr], #4
	SMLAL a2,v3,a4,a1
	add	ip, ip, #1
	ldr	v2, [sp, #4]
	cmp	ip, v2
	blt	|L..103|

which means not only is it moving a3 into v4 for the SMLAL operation, but
also it's doing that wierd double-move through a4.

Any ideas?
*/

typedef signed long Fixed;
typedef struct {     unsigned long lo;  signed long hi; } FixedD;

#define F_mla_wide_noi(a,b,c) asm("SMLAL %0,%1,%2,%3" \
	: "=&r" (c.lo), "=&r" (c.hi) \
	: "%r" (a), "%r" (b), "0" (c.lo), "1" (c.hi) )

Fixed input, input1;
FixedD FD_0;

void silly (int x, int y)
{
  printf ("hello, x = %d, y = %d\n", x, y);
}

void fucked (int x)
{
   Fixed *ip, *wp;
    register FixedD s;
    int j;

    ip = input;
    wp = input1; 
    s=FD_0;
    for (j = 0; j < x; j++)
    {
      Fixed a,b;

      a=*wp++;
      b=*ip++;
      /* asm ("SMLAL %0,%1,%2,%3" : "=&r" (s.lo), "=&r" (s.hi)
      	   : "%r" (a), "%r" (b), "0" (s.lo), "1" (s.hi)); */
      /* asm ("SMLAL %0,%1,%2,%3" : "=r" (s.lo), "=r" (s.hi)
      	   : "r" (a), "r" (b), "r" (s.lo), "r" (s.hi));
      /* asm ("SMLAL %0,%1,%2,%3" : "=&r" (s.lo), "=&r" (s.hi)
      	   : "r" (a), "r" (b), "r" (s.lo), "r" (s.hi)); */
      asm ("SMLAL %0,%1,%2,%3" : "=r,r" (s.lo), "=r,r" (s.hi)
      	   : "%r,r" (a), "%r,r" (b), "r,r" (s.lo), "r,r" (s.hi));
      if (j == s.lo)
        j += 5;
      /* F_mla_wide_noi(a,b,s); */
    }
}

int main (void)
{
  fucked (52);
  return 0;
}
