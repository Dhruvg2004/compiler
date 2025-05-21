section .data
	printf_format: db '%d', 10, 0
section .text
	global main
	extern printf
main:
	mov rdi, 2
	mov rdi, 5
	mov rdi, 3
	mov rax, 60
	mov rdi, 3
	syscall
	mov rax, 60
	mov rdi, 2
	syscall
