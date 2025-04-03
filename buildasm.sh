nasm -f elf64 generate.asm -o generate.o
gcc generate.o -o generate -lc -no-pie
./generate
echo $?
