#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

int
main(int argc, char** argv)
{
  char *a;
  int fd, n;

  char s[] = "rwtest";

  a = sbrk(PGSIZE);
  fd = open("sbrk", O_CREATE|O_WRONLY);
  if(fd < 0)  {
    printf("%s: open sbrk failed\n", s);
    exit(1);
  }
  if ((n = write(fd, a, PGSIZE)) < 0) {
    printf("%s: write sbrk failed\n", s);
    exit(1);
  }
  close(fd);

  a = sbrk(PGSIZE);
  fd = open("sbrk", O_RDONLY);
  if(fd < 0)  {
    printf("%s: open sbrk failed\n", s);
    exit(1);
  }
  if ((n = read(fd, a, PGSIZE)) < 0) {
    printf("%s: write sbrk failed\n", s);
    exit(1);
  }
  close(fd);
  unlink("sbrk");

  a = sbrk(2*PGSIZE);

  /* int i = 4069; */
  /* a[i++] = 'H'; */
  /* a[i++] = 'A'; */
  /* a[i++] = 'H'; */
  /* a[i++] = 'A'; */
  /* a[i++] = '!'; */
  /* a[i++] = '\0'; */
  /* a[i++] = 'e'; */
  /* a[i++] = 'c'; */
  /* a[i++] = 'h'; */
  /* a[i++] = 'o'; */
  /* a[i++] = '\0'; */
  /* *(char**)(a+i++) = &a[4075]; */
  /* *(char**)(a+i++) = &a[4069]; */
  /* char** uargv = (char**)(a+4080); */

  a[PGSIZE-5] = 'H';
  a[PGSIZE-4] = 'A';
  a[PGSIZE-3] = 'H';
  a[PGSIZE-2] = 'A';
  a[PGSIZE-1] = '!';
  char** uargv = (char**)a;
  uargv[0] = &a[20];
  uargv[1] = &a[PGSIZE-5];
  uargv[2] = 0;

  a[30] = 'e';
  a[31] = 'c';
  a[32] = 'h';
  a[33] = 'o';
  a[34] = '\0';
  exec("echo", uargv);

  printf("This should never be displayed.\n");

  exit(0);
}

