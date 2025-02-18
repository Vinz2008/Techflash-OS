/*
	Original version Copyright 2017 - 2021 bzt (bztsrc@gitlab)
	as part of the 'bootboot' repository on GitLab.
	Original file available at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/kernel.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <kernel/environment.h>
#include <kernel/tty.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/parallel.h>
#include <kernel/hardware/PIT.h>
#include <kernel/boot.h>
#include <kernel/graphics.h>
#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/panic.h>
#include <kernel/hardware/kbd.h>
#include <kernel/fs/tar.h>
#include <kernel/shell.h>
#include <kernel/memory.h>
#include <kernel/elf.h>
#include <kernel/syscall.h>

#include <kernel/custom.h>

MODULE("KERNEL");
kernel_t kernel;

uint8_t SSEFeaturesBits = 0;
void initThings();
void initExceptions();
void PICInit();
void keyboardInit();
extern bool timerReady;
static uint8_t whatCoreAmI() {
	uint64_t rbx = 0;
	asm (
		"mov $1, %%rax;"
		"cpuid;"
		"movq %%rbx, %0;"
		: "=r" (rbx)
		:
	);
	return (rbx >> 24);
}
// cppcheck-suppress unusedFunction
void kernelMain() {
	// before we do anything, if we're not the bootstrap processor, just hang.
	// in future versions, they'll do things, for now, they won't
	// FIXME: This fixes crashing on SMP systems, but not in a good way, we really shouldn't be running single threaded.
	if (whatCoreAmI() != bootboot.bspid) {
		// we are an non-bootstrap processor, halt
		while (true) {
			asm ("cli; hlt");
		}
	}
	
	/*** NOTE: this code runs on all cores in parallel ***/
	// int s = bootboot.fb_scanline;
	// int w = bootboot.fb_width;
	// int h = bootboot.fb_height;
	SSEInit();
	// This is safe here because all it does it set a variable.
	mallocInit((void*)0x0000000001000000);
	initThings();
	// initialize serial logging.
	serial.init(115200);

	keyboard.setLED(KEYBOARD_LED_NUMLOCK, true);

	// Say that the kernel is loading and to please wait.
	puts("Techflash OS v");
	serial.writeString(SERIAL_PORT_COM1, "Techflash OS v");
	TTY_Color = colors.vga.cyan + 0x002020;
	printf("%d.%d.%d", CONFIG_KERN_VERSION_MAJOR, CONFIG_KERN_VERSION_MINOR, CONFIG_KERN_VERSION_PATCH);
	TTY_Color = colors.vga.lgray;

	serial.write(SERIAL_PORT_COM1, CONFIG_KERN_VERSION_MAJOR + '0');
	serial.write(SERIAL_PORT_COM1, '.');
	serial.write(SERIAL_PORT_COM1, CONFIG_KERN_VERSION_MINOR + '0');
	serial.write(SERIAL_PORT_COM1, '.');
	serial.write(SERIAL_PORT_COM1, CONFIG_KERN_VERSION_PATCH + '0');

	puts(" Loading...\r\n");
	serial.writeString(SERIAL_PORT_COM1, " Loading...\r\n");

	// if (env.experimental.progressBarBoot) {
	uint8_t bootX = ((TTY_Width / 2) - (TTY_Width / 3)); // idk it looks centered to me
	uint8_t bootY = ((TTY_Height / 2) + (TTY_Height / 4)); // don't forget, the Y goes up the farther down the screen you are.  this means 3/4 down the screen
	BP_Init(bootX, bootY, TTY_Width / 2);

	// Initialize the 8259 Programmable Interrupt Controller
	PICInit();
	BP_Update();

	// Initialize the Global Descriptor Table
	GDTInit();
	BP_Update();

	// Initialize the Interrupt Descriptor Table
	IDTInit();
	BP_Update();

	// Init the keyboard driver
	keyboardInit();
	BP_Update();

	// Initialize some exception handlers
	initExceptions();
	BP_Update();

	// Initialize the PIT to once every 1ms
	initPIT(1000);
	BP_Update();
	
	keyboard.setIntState(0, true);
	
	// Now the interrupts are ready, enable them
	log(MODNAME, "INTERRUPTS ARE BEING ENABLED!!!", LOGLEVEL_WARN);
	asm volatile ("sti");
	timerReady = true;
	BP_Update();

	// Initialize the parallel port (we need interrupts for this, since it has a timeout)
	parallel.init();
	BP_Update();

	initSyscalls();
	BP_Update();
	parseTar((void *)bootboot.initrd_ptr);
	BP_Update();
	handleEnv();
	
	sleep(250);
	
	BP_FadeOut();
	TTY_PrintPrompt();
	TTY_BlinkingCursor = true;
	TTY_CursorAfterPromptX = 0;
	char *command = malloc(512);
	uint16_t commandStrIndex = 0;
	DUMPREGS;
	panic("a", regs);
	while (true) {
		// Main kernel loop
		char userInput = keyboard.getLastKey();
		if ((uint8_t)userInput == 0xFF) {
			// Special key
			char *specialKey = keyboard.getLastSpecialKey();
			if (specialKey[0] == '\r' && specialKey[1] == '\n') {
				// Enter
				puts("\r\n");
				command[commandStrIndex] = '\0';
				uint8_t val = handleCommands(command);
				if (val == 1) {
					printf("Unknown command: \'%s\'\r\n", command);
				}
				memset(command, 0, commandStrIndex); // zero out the string

				commandStrIndex = 0;
				TTY_PrintPrompt();
			}
			else if (specialKey[0] == '\b') {
				// Backspace
				if (TTY_CursorAfterPromptX != 0) {
					putcAt(' ', TTY_CursorX - 1, TTY_CursorY, TTY_Color);
					TTY_CursorX--;
					TTY_CursorAfterPromptX--;
					commandStrIndex--;
				}
			}
		}
		if (userInput != (char)'\0' && (uint8_t)userInput != 0xFF) {
			putchar(userInput);
			command[commandStrIndex] = userInput;
			commandStrIndex++;
		}
	}
	asm volatile (
		"cli\n"
		"hlt\n"
	);
}
