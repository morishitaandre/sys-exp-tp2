#include "kernel/types.h"
#include "kernel/memlayout.h"
#include "user/user.h"


char code[] = {
  // li a0, 1
  0x05, 0x45,
  // li a7, 16
  0xc1, 0x48,
  // auipc a1, 0
  0x97, 0x05, 0x00, 0x00,
  // .loop:
  // lbu t0, 0(a1)
  0x83, 0xc2, 0x05, 0x00,
  // addi a1, a1, 1
  0x85, 0x05,
  //li t1, 0x80
  0x13, 0x03, 0x00, 0x08,
  // beq t0 t1, .endloop
  0x63, 0x83, 0x62, 0x00,
  // j .loop
  0xcd, 0xbf,
  // .endloop:
  // mv a2, a1
  0x2e, 0x86,
  // .loop2:
  // lbu t0, 0(a2)
  0x83, 0x42, 0x06, 0x00,
  // addi a2, a2, 1
  0x05, 0x06,
  // beqz t0, .endloop2
  0x63, 0x83, 0x02, 0x00,
  // j .loop2
  0xdd, 0xbf,
  // .endloop2
  // sub a2, a2, a1
  0x0d, 0x8e,
  // ecall
  0x73, 0x00, 0x00, 0x00,
  // ret
  0x82, 0x80,
  'H', 'e', 'l', 'l', 'o', '!', 0x0a, 0
};


int
main(int argc, char *argv[])
{

  /*
    # test.S
    # riscv64-unknown-elf-as -march=rv64gc -fpic test.S
    # riscv64-unknown-elf-objdump -d /tmp/a.out

    # write(1, s, sizeof(s))

    # fd = 1
    li a0, 1
    # syscall write -> a7 = 16
    li a7, 16

    # looking for the address of the string to print, after ret (0x82, 0x80)

    auipc a1, 0
    .loop:
    lbu t0, 0(a1)
    addi a1, a1, 1
    li t1, 0x80
    beq t0, t1, .endloop
    j .loop
    .endloop:

    # computing the length of the string, look for 0 byte
    mv a2, a1
    .loop2:
    lbu t0, 0(a2)
    addi a2, a2, 1
    beq t0, zero, .endloop2
    j .loop2
    .endloop2:
    sub a2, a2, a1

    ecall

    ret

    .mystring:
    auipc a1, 4
    ret
    "Hello"
    .mystringend:
    auipc a2, -4
    ret

   */

  char code_stack[100];
  for(int i = 0; i < sizeof(code); i++){
    code_stack[i] = code[i];
  }

  int pid = fork();
  if(pid < 0){
    printf("fork failed\n"); exit(1);
  }
  if(pid == 0){
    ((void(*)(void))(code_stack))();
    printf("I successfully ran the stack.\n");
    exit(0);
  }
  else {
    // in .rodata
    pid = fork();
    if(pid < 0){
      printf("fork failed\n"); exit(1);
    }
    if(pid == 0){
      ((void(*)(void))(code))();
      printf("I successfully ran the rodata.\n");
      exit(0);
    }
    else {
      wait(0);
      wait(0);
    }
  }
  exit(0);
}
