set -e
nasm -f elf stdio.asm
nasm -f elf test.asm
ld -m elf_i386 -o test test.o stdio.o

chmod +x test
./test