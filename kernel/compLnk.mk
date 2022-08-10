compile=\
kernel.o \
init.o \
boot.o \
panic.o \
environment.o \
tty/tty.o \
hardware/IO/in.o \
hardware/IO/out.o \
hardware/IO/wait.o \
hardware/kbd.o \
hardware/kbdASM.o \
hardware/serial/serialRW.o \
hardware/serial/serialWriteLoop.o \
hardware/FPU.o \
hardware/CPU/SSE.o \
hardware/CPU/interrupts/GDT.o \
hardware/CPU/interrupts/GDTASM.o \
hardware/CPU/interrupts/IDT.o \
hardware/CPU/interrupts/IDTASM.o \
hardware/CPU/interrupts/exceptionHandler.o \
hardware/CPU/interrupts/ISRASM.o \
hardware/CPU/interrupts/ISR.o \
hardware/CPU/interrupts/PIC.o \
hardware/PIT/PIT.o \
graphics/debug.o \
graphics/pixel.o \
graphics/vga.o

link=\
../build/kernel/kernel.o \
../build/kernel/init.o \
../build/kernel/boot.o \
../build/kernel/panic.o \
../build/kernel/environment.o \
../build/kernel/tty/tty.o \
../build/kernel/graphics/debug.o \
../build/kernel/graphics/pixel.o \
../build/kernel/graphics/vga.o \
../build/kernel/hardware/IO/in.o \
../build/kernel/hardware/IO/out.o \
../build/kernel/hardware/IO/wait.o \
../build/kernel/hardware/kbd.o \
../build/kernel/hardware/kbdASM.o \
../build/kernel/hardware/PIT/PIT.o \
../build/kernel/hardware/serial/serialRW.o \
../build/kernel/hardware/serial/serialWriteLoop.o \
../build/kernel/hardware/CPU/SSE.o \
../build/kernel/hardware/FPU.o \
../build/kernel/hardware/CPU/interrupts/GDT.o \
../build/kernel/hardware/CPU/interrupts/GDTASM.o \
../build/kernel/hardware/CPU/interrupts/IDT.o \
../build/kernel/hardware/CPU/interrupts/IDTASM.o \
../build/kernel/hardware/CPU/interrupts/exceptionHandler.o \
../build/kernel/hardware/CPU/interrupts/ISRASM.o \
../build/kernel/hardware/CPU/interrupts/ISR.o \
../build/kernel/hardware/CPU/interrupts/PIC.o \
../build/kernel/font.o
