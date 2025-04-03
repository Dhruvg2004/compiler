section .data
  printf_format: db '%d', 10, 0
extern printf
global main
section .text
main:
  mov rax, 60
  mov rdi, 1
  syscall
