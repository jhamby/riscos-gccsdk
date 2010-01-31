/*
 * Test function for getgrouplist()
 *
 * If built for RISC OS, will use the group file in /etc/group:
 * need to copy the file test.groups there before running the test
 * (and probably replace it afterwards with what was there before)
 *
 * If built for other platforms it'll use the host C library functions to
 * read the file 'test.group' in the current directory.  It'll only test
 * the copy of getgrouplist() found in this file, not the one in
 * the main UnixLib tree.
 *
 * Copyright (c) 2008 UnixLib Developers
 */

/* if not on RISC OS include our own copy of the function */
#ifndef __riscos
#include "../../grp/getgrouplist.c"
#else
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>
#include <pthread.h>
#endif

#define IN(a,b,c,d) printf("Calling getgrouplist(\"%s\",%d,%x,%x) ",a,b,c,d);\
if (d) printf(" ngroups=%d ",*d);
#define OUT(r) printf(" result=%d ",r);
#define GETGROUPLIST(a,b,c,d,e) IN(a,b,c,d);e=getgrouplist(a,b,c,d);OUT(e)
#define TEST(a,b,f) {if(a==b) printf(" PASS! Expected %s=%s\n",#a,#b); else {printf(" FAIL! Expected %s=%s!\n",#a,#b);f=1;}}
#define SHOWGROUPS(grps,ngrps) { int i=0; for (i=0; i<ngrps; i++) {printf("GID %d = %d\n",i,grps[i]);}}

int main(void)
{
  char *user=NULL;
  gid_t group=0;
  gid_t groups[100];
  int ngroups;
  int result=0;
  int *n=NULL;
  int fail=0;

/* these tests won't work unless you have the right group file in place - see above */
#ifdef __riscos
  fprintf(stderr,"libunixlib/test/grp/getgrouplist.c will only work if you have\n"\
  "copied in libunixlib/test/grp/test.group into /etc/group on RISC OS.\n" \
  "Make sure you restore /etc/group afterwards.\n");
#endif
  
  GETGROUPLIST(user,group,n,n,result);
  TEST(result,EINVAL,fail)
  
  GETGROUPLIST(user,group,groups,n,result);
  TEST(result,EINVAL,fail);
    
  user="zzzz";
  GETGROUPLIST(user,group,groups,n,result);
  TEST(result,EINVAL,fail);

  ngroups=0;
  GETGROUPLIST(user,group,groups,&ngroups,result);
  TEST(result,EINVAL,fail);

  ngroups=3;
  user="missing";
  GETGROUPLIST(user,group,groups,&ngroups,result);
  TEST(result,0,fail);
  TEST(ngroups,0,fail);

  ngroups=3;
  user="bill";
  group=(gid_t) 123;
  GETGROUPLIST(user,group,groups,&ngroups,result);
  TEST(result,-1,fail);
  TEST(ngroups,3,fail);
  SHOWGROUPS(groups,ngroups);  

  ngroups=3;
  user="fred";
  group=(gid_t) 123;
  GETGROUPLIST(user,group,groups,&ngroups,result);
  TEST(result,3,fail);
  TEST(ngroups,3,fail);
  SHOWGROUPS(groups,ngroups);  

  ngroups=20;
  user="bill";
  group=(gid_t) 123;
  GETGROUPLIST(user,group,groups,&ngroups,result);
  TEST(result,6,fail);
  TEST(ngroups,6,fail);
  SHOWGROUPS(groups,ngroups);  

  ngroups=20;
  user="bill";
  group=(gid_t) 44;
  GETGROUPLIST(user,group,groups,&ngroups,result);
  TEST(result,5,fail);
  TEST(ngroups,5,fail);
  SHOWGROUPS(groups,ngroups);  

  ngroups=5;
  user="bill";
  group=(gid_t) 44;
  GETGROUPLIST(user,group,groups,&ngroups,result);
  TEST(result,5,fail);
  TEST(ngroups,5,fail);
  SHOWGROUPS(groups,ngroups);  

  return fail;
}

