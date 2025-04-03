section .data
	printf_format: db '%d', 10, 0
section .text
	global main
	extern printf
main:
	mov rax, 60
	mov rdi, 76
	syscall
