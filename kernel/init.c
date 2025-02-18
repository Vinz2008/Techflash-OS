#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <kernel/font.h>
#include <kernel/bda.h>
#include <kernel/tty.h>
#include <kernel/hardware/CPU/x86Setup.h>

extern void elfInit();
registers_t regsDump;

extern void keyboardInit();
extern bool serialInit(uint64_t speed);
extern void colorsInit();
extern void parallelInit();
bda_t *bda;
bool heapSetUp;
bool haveAllocated;
unsigned char *lastValidHeapAddress;
unsigned char *heapSpace;

psf2_t *font;
extern char *arguments;
bool timerReady;
// calls the init functions to initialize the function pointers for all of the structs
void initThings() {
	font = (psf2_t*)&_binary_font_psf_start;
	bda = (void *)0x400;
	elfInit();
	timerReady = false;

	// Start setting up the serial struct.
	serialInit(0);

	// initialize the parallel struct.
	parallelInit();

	// initialize the colors struct
	colorsInit();
	
	// Start initializing a TTY.
	TTY_Init();

	// Start setting up the keyboard struct.
	keyboardInit();

	arguments = malloc(128);
}