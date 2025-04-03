section .data
    printf_format: db "%d", 10, 0  ; "%d\n" format string (null-terminated)

section .text
    global main
    extern printf

main:
    sub rsp, 8              ; Align stack (System V ABI requires 16-byte alignment)
    
    mov edi, printf_format  ; 1st argument: format string
    mov esi, 42             ; 2nd argument: integer value (to print)
    xor eax, eax            ; Clear eax for varargs compatibility
    call printf             ; Call printf from libc

    add rsp, 8              ; Restore stack alignment
    mov eax, 0              ; Return 0
    ret                     ; Return to caller
