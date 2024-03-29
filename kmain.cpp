#include <cputables.h>
#include <console.h>

extern "C" void kmain( void* mbd, unsigned int magic ){
	if ( magic != 0x2BADB002 )
	{
		/* Something went not according to specs. Print an error */
		/* message and halt, but do *not* rely on the multiboot */
		/* data structure. */
	}

	/* You could either use multiboot.h */
	/* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
	/* or do your offsets yourself. The following is merely an example. */ 
	//char * boot_loader_name =(char*) ((long*)mbd)[16];

	/* Print a letter to screen to see everything is working: */
	//unsigned char *videoram = (unsigned char *) 0xb8000;
	//videoram[0] = 65; /* character 'A' */
	//videoram[1] = 0x07; /* light grey (7) on black (0). */

	VideoRamConsole << "I am testing the scrolling behaviour of the console.\n";
	VideoRamConsole << "Let's first try a line with a lot lot lot lot lot lot lot lot lot lot lot of characters. You know what I mean?\n";
	VideoRamConsole << "Now let's iterate over about 25 lines";
	for (int i = 0; i < 25; i++)
		VideoRamConsole << "Iteratin...\n";
	VideoRamConsole << "Will now print something on the bottom, hopefully\n";
	VideoRamConsole << "Let's first try a line with a lot lot lot lot lot lot lot lot lot lot lot of characters. You know what I mean?\n";

	c << "Loading GDT...";
	load_gdt();
	c << "done\n";
	c << "Loading IDT...";
	load_idt();
	c << "done\n";
}
