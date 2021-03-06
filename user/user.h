#ifndef USER_H
#define USER_H
#include "kernel/param.h"

struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int sclose(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(uint64);
int sleep(int);
int uptime(void);
int ntas();
int nice(int, int);
int crash(const char*, int);
int mount(char*, char *);
int umount(char*);

int create_mutex();
int acquire_mutex(int fd);
int release_mutex(int fd);

int dump_pagetable(int pid);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...);
void printf(const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);

void fflush(int fd);

#define PUTC_BUF_LEN 4096

extern char* putc_buf[];
extern int putc_index[];
char* get_putc_buf(int fd);

#endif
