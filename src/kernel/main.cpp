#include "inc/main.hpp"
#include "../inc/util.hpp"
extern "C" {
	extern void printTest();
}
void kernelMain() {
	print((const uint_fast8_t*)".", 9, 0);
	print((const uint_fast8_t*)"Kernel Loaded", 0, 1, GREEN);
	print((const uint_fast8_t*)"> ", 0, 2); // Print a prompt
	terminal.cursor.enable(15, 15);
	terminal.cursor.position.change(1, 2);
	halt();
	return;
}
