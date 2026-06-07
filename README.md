# BasicMicrokernel
A simple microkernel to use in OS subject and run in CodeSpace.

Buildar e compilar:
clear && make && qemu-system-riscv64 -machine virt -m 128M -nographic -bios default -kernel kernel.elf