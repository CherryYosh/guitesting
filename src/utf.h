#ifndef UTF_H
#define UTF_H

unsigned int utfGetError();

class utfChar{
public:
	utfChar();
	utfChar( wchar_t );
	utfChar( unsigned char );
	utfChar( unsigned char* );
	utfChar( unsigned short );
	utfChar( unsigned int );
	utfChar( unsigned long );

	~utfChar();

	utfChar operator=( utfChar );
	bool operator==( utfChar );
	bool operator!=( utfChar );

	unsigned char Size();
private:
	unsigned char data[];
};

#endif
