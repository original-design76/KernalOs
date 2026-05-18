KernalOs
A custom 32-bit x86 Operating System kernel with a Windows 1.0 / MS-DOS Executive inspired GUI.

Features
Custom Bootloader: Uses NASM and Multiboot standard.

VGA Text Mode GUI: Hand-written driver for 80x25 color display.

Keyboard Driver: Direct I/O port communication for arrow key navigation.

Windows 1.0 Aesthetics: Classic blue and white "MS-DOS Executive" style interface.

How to Build and Run
1. Prerequisites
You will need a Linux environment (like WSL Ubuntu) and the following tools:

Bash
sudo apt update
sudo apt install nasm gcc binutils qemu-system-x86 xorriso grub-pc-bin mtools -y
2. Compile and Link
Run these commands in your terminal to turn the source code into a bootable binary:

Bash
# Assemble the bootloader
nasm -felf32 boot.asm -o boot.o

# Compile the C kernel
gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# Link everything together
gcc -m32 -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
3. Create a Bootable ISO
To run this on VirtualBox or real hardware, you need an ISO file:

Bash
mkdir -p isodir/boot/grub
cp myos.bin isodir/boot/
cat <<EOF > isodir/boot/grub/grub.cfg
menuentry "Kernel OS" {
	multiboot /boot/myos.bin
}
EOF
grub-mkrescue -o myos.iso isodir
4. Run in QEMU
You can test the OS instantly using QEMU:

Bash
qemu-system-i386 -kernel myos.bin
(Note: If using WSL without a display server, add -display curses to the command above.)

Project Structure
boot.asm: The entry point that handles the transition from the bootloader to our C code.

kernel.c: The main logic, including VGA rendering and keyboard input.

linker.ld: Tells the compiler exactly where to place code in memory.
