/* setjmp-alloca.c
   Tests the interaction between setjmp/longjmp and alloca.
   Written by John Tytgat <John.Tytgat@aaug.net>  */

#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#define SETJMP_VALUE0 (230)
#define ALLOCA_SIZE (10*1024*1024)

static jmp_buf buf0;

void do_test1(void)
{
  int val0;
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test1(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test1(): after setjmp()\n");
  longjmp(buf0, SETJMP_VALUE0);
  printf("do_test1(): after longjmp(), we should never reach here\n");
  abort();
}

void do_test2(void)
{
  void *ptr2 = alloca(ALLOCA_SIZE);
  int val0;
  printf("do_test2(): alloca() returned %p\n", ptr2);
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test2(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test2(): after setjmp()\n");
  longjmp(buf0, SETJMP_VALUE0);
  printf("do_test2(): after longjmp(), we should never reach here\n");
  abort();
}

void do_test3(void)
{
  void *ptr3;
  int val0;
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test3(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test3(): after setjmp()\n");
  ptr3 = alloca(ALLOCA_SIZE);
  printf("do_test3(): alloca() returned %p\n", ptr3);
  longjmp(buf0, SETJMP_VALUE0);
  printf("do_test3(): after longjmp(), we should never reach here\n");
  abort();
}

void do_longjmp_no_alloc(void)
{
  printf("do_longjmp_no_alloc(): about to longjmp()\n");
  longjmp(buf0, SETJMP_VALUE0);
  printf("do_longjmp_no_alloc(): after longjmp(), we should never reach here\n");
  abort();
}

void wrap_longjmp_no_alloc(void)
{
  printf("wrap_longjmp_no_alloc()\n");
  do_longjmp_no_alloc();
}

void do_longjmp_alloc(void)
{
  void *ptr = alloca(ALLOCA_SIZE);
  printf("do_longjmp_alloc(): alloca() returned %p\n", ptr);
  printf("do_longjmp_alloc(): about to longjmp()\n");
  longjmp(buf0, SETJMP_VALUE0);
  printf("do_longjmp_alloc(): after longjmp(), we should never reach here\n");
  abort();
}

void wrap_longjmp_alloc(void)
{
  printf("wrap_longjmp_alloc()\n");
  do_longjmp_alloc();
}

void do_test4(void)
{
  int val0;
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test4(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test4(): after setjmp()\n");
  do_longjmp_no_alloc();
}

void do_test5(void)
{
  int val0;
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test5(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test5(): after setjmp()\n");
  do_longjmp_alloc();
}

void do_test6(void)
{
  void *ptr6 = alloca(ALLOCA_SIZE);
  int val0;
  printf("do_test6(): alloca() returned %p\n", ptr6);
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test6(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test6(): after setjmp()\n");
  do_longjmp_no_alloc();
}

void do_test7(void)
{
  void *ptr7 = alloca(ALLOCA_SIZE);
  int val0;
  printf("do_test7(): alloca() returned %p\n", ptr7);
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test7(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test7(): after setjmp()\n");
  do_longjmp_alloc();
}

void do_test8(void)
{
  void *ptr8;
  int val0;
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test8(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test8(): after setjmp()\n");
  ptr8 = alloca(ALLOCA_SIZE);
  printf("do_test8(): alloca() returned %p\n", ptr8);
  do_longjmp_no_alloc();
}

void do_test9(void)
{
  void *ptr9;
  int val0;
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test9(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test9(): after setjmp()\n");
  ptr9 = alloca(ALLOCA_SIZE);
  printf("do_test9(): alloca() returned %p\n", ptr9);
  do_longjmp_alloc();
}

void do_test10(void)
{
  int val0;
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test10(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test10(): after setjmp()\n");
  wrap_longjmp_alloc();
}

void do_test11(void)
{
  void *ptr11 = alloca(ALLOCA_SIZE);
  int val0;
  printf("do_test11(): alloca() returned %p\n", ptr11);
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test11(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test11(): after setjmp()\n");
  wrap_longjmp_no_alloc();
}

void do_test12(void)
{
  void *ptr12 = alloca(ALLOCA_SIZE);
  int val0;
  printf("do_test12(): alloca() returned %p\n", ptr12);
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test12(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test12(): after setjmp()\n");
  wrap_longjmp_alloc();
}

void do_test13(void)
{
  void *ptr13;
  int val0;
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test13(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test13(): after setjmp()\n");
  ptr13 = alloca(ALLOCA_SIZE);
  printf("do_test13(): alloca() returned %p\n", ptr13);
  wrap_longjmp_no_alloc();
}

void do_test14(void)
{
  void *ptr14;
  int val0;
  if ((val0 = setjmp(buf0)) != 0)
    {
      printf("do_test14(): setjmp() succeeds: rtrn is %d (expected %d)\n", val0, SETJMP_VALUE0);
      if (val0 != SETJMP_VALUE0)
        abort();
      return;
    }
  printf("do_test14(): after setjmp()\n");
  ptr14 = alloca(ALLOCA_SIZE);
  printf("do_test14(): alloca() returned %p\n", ptr14);
  wrap_longjmp_alloc();
}

#define TESTS  \
  do_test1();  \
  do_test2();  \
  do_test3();  \
  do_test4();  \
  do_test5();  \
  do_test6();  \
  do_test7();  \
  do_test8();  \
  do_test9();  \
  do_test10(); \
  do_test11(); \
  do_test12(); \
  do_test13(); \
  do_test14();

void do_wrap_no_alloc(void)
{
  printf("do_wrap_no_alloc()\n");
  TESTS
}

void do_wrap_alloc(void)
{
  void *ptr = alloca(ALLOCA_SIZE);
  printf("do_wrap_alloc(): alloca() returned %p\n", ptr);
  TESTS
}

int main(int argc, char *argv[])
{
  TESTS

  do_wrap_no_alloc();
  do_wrap_alloc();

  printf("Success (if there are no memory leaks)!\n");
  return EXIT_SUCCESS;
}
