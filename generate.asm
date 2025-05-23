section .data
	printf_format: db '%d', 10, 0
section .text
	global main
	extern printf
main:
	push 5
	mov rax, 60
	mov rdi, 14
	syscall
