#include <stdint.h>
#include <property.h>

#ifndef __CONSOLE_H
#define __CONSOLE_H

class BufferConsole
{
	private:
		uint16_t* buffer; //This is probably the address of video memory
		//Some constants for a vga console
		int32_t LineWidth;
		int32_t NLines;
		
		int32_t cursor; //the seek pointer
		
		uint8_t m_fgColour;
		uint8_t m_bgColour;

		int32_t lineCoordCursor;
		int32_t columnCoordCursor;

		void scroll(); //Nove the screen down by nlines
		void newline(); //Move the cursor to a new line, maybe scrolling`
		uint16_t getColourAttrib(); //16 bit, so we can superimpose a char on it
	public:
		BufferConsole(void* buffer);
		
		BufferConsole& write(const char* s, uint32_t n); //write n chars
		BufferConsole& write(const char* s); //write C string (null terminated)

		BufferConsole& put(char c); //write the character, not the value of character

		BufferConsole& operator<<(const char* s);
		BufferConsole& operator<<(int32_t val);
		BufferConsole& operator<<(uint32_t val);
		BufferConsole& operator<<(int16_t val);
		BufferConsole& operator<<(uint16_t val);
		BufferConsole& operator<<(int8_t val);
		BufferConsole& operator<<(uint8_t val);
		BufferConsole& operator<<(float val);
		BufferConsole& operator<<(double val);

		int32_t tellp(); //get position of stream pointer
		BufferConsole& seekp(int32_t pos); //set seek position

		property<BufferConsole,uint8_t,PROP_READ_WRITE> fgColour;
		property < BufferConsole, uint8_t, PROP_READ_WRITE > bgColour;
};

#endif
