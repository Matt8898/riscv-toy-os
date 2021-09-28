meson --cross-file riscv.cross build
cd build
ninja
cd ..

riscv64-elf-objcopy -j .init -j .text -j .bss -j .rodata -j .data --set-section-flags .bss=alloc,load,contents -O binary build/kernel build/kernel.bin
mkimage -d build/kernel.bin -A riscv -O Linux -a 0x45000000 -e 0x45000000 -C none build/boot.img
mkbootimg --kernel ./build/boot.img   --cmdline "test" --base 0x45000000 --kernel_offset 0 -o boot.img
