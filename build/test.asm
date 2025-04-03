section .data
	printf_format: db '%d', 10, 0
section .text
	extern printf
	global main
main:
 	mov rax, 60
	mov rdi, 1
	syscall
