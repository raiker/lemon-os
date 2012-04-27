#include <stdint.h>

#define ENC_UTF8 8
#define ENC_UTF16 16

class String
{
	public:
		String(); //empty string
		String(Character* charArray, uint32_t nChars); //initialise from character array
		String(char* str); //from null-terminated ascii string
		String(char* utf8Str, int encoding); //from utf8 data; with encoding must be ENC_UTF8
		
		~String(); //sort of want release that array
		
		uint32_t encodeAsUTF8(char* buff, uint32_t bufLen); //write the string as utf8 into buff, returns number of bytes written or 0 if not big enough
		uint32_t encodeAsCStr(char* buff, uint32_t bufLen); //wruite the string as a C string; returns number of bytes written or 0 if buf not big enough. Invalid becomes 0.
		
		Character* getCharacters();
		
		uint32_t len();
		String substring(uint32_t startPos, uint32_t substrLength); //substring.
	private:
		Character[] characters;
		uint32_t nCharacters;
}

class Character
{
	public:
		Character(); //Initialise as null character
		Character(char c); //Initialise as this ascii character
		Character(uint32_t codePoint); //initialise with unicode code point
		Character(char* utf8Char); //Initialise with null-terminated utf-8 encoded code point
		Character(char* utf8Char, uint8_t encLen); //Initialise with null-terminated utf-8 code point of length encLen bytes
		
		uint32_t getCodePoint();
		
		uint32_t encodeAsUTF8(char* buff, uint32_t bufLen); //write the character as utf8 into buff; returns number of bytes written or 0 if not big enough buff
		
		operator char(); //return an ascii representation or NULL on epic flail
		operator uint32_t(); //this just is getCodePoint
	private:
		uint32_t codePoint; //the unicode code point of this character
}