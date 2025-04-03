nasm -f elf64 test.asm -o test.o
gcc test.o -o test -lc -no-pie
