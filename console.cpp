#include "console.h"

BufferConsole VideoRamConsole((void*)0xb8000);

BufferConsole::BufferConsole(void* buffer) : fgColour(this, &m_fgColour), bgColour(this, &m_bgColour)
{
	this->buffer = (uint16_t*)buffer;
	this->fgColour = 0xF;
	this->bgColour = 0x0;
	this->cursor = 0;
	this->lineCoordCursor = 0;
	this->columnCoordCursor = 0;
	this->LineWidth = 80;
	this->NLines = 25;
}

void BufferConsole::scroll()
{
	//Iterate through every line in the buffer, and set it equal to the line below
	for (int i = 0; i < NLines - 1; i++)
	{
		//iterate through every character in this line
		for (int j = 0; j < LineWidth; j++)
		{
			buffer[i*LineWidth+ j] = buffer[(i+1)*LineWidth + j]; //copy char colour and all
		}	
	}
	//clear the last line to empty
	for (int i = 0; i < LineWidth; i++)
	{
		//going to assume null character clears the char
		uint16_t attrib = this->getColourAttrib();
		buffer[(NLines-1)*LineWidth + i] = 0x0 | attrib;
	}
	//Move the cursor back to the begining of this line
	columnCoordCursor = 0;
	return;
}


uint16_t BufferConsole::getColourAttrib()
{
	uint16_t retval = (bgColour << 4) | (fgColour & 0x0F);
	return (retval << 8);
}

void BufferConsole::newline()
{
	if (lineCoordCursor < (NLines - 1))
	{
		//Not on the bottom line. Just bump the cursor down
		lineCoordCursor++;
		columnCoordCursor = 0;
	}
	else if (lineCoordCursor == (NLines - 1))
	{
		//We're on the bottom line. Scroll
		this->scroll();
	}
	return;
}


BufferConsole& BufferConsole::put(char c)
{
	switch (c)
	{
		case '\n':
			this->newline();
			cursor++; //the logical one
			break;
		default:
			//we may need a newline anyway
			if (columnCoordCursor == (LineWidth - 1))
				this->newline();
			uint16_t attrib = this->getColourAttrib();
			buffer[lineCoordCursor * LineWidth + columnCoordCursor] = c | attrib;
			columnCoordCursor++;
			cursor++;
	}
	return *this;
}

BufferConsole& BufferConsole::write(const char* s)
{
	int i = 0;
	while (s[i] != 0x0)
	{
		this->put(s[i]);
		i++;
	}
	return *this;
}

BufferConsole& BufferConsole::operator<<(const char* s)
{
	return this->write(s);
}
