/*   This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	long with this program.  If not, see <http://www.gnu.org/licenses/>

 * 	Copyright 2008,2009 James Brandon Stevenson
 */
/*
#include "utf.h"

#define UTF_INVALID 1

unsigned int _error = 0;

utfChar::utfChar(){
	size = 0;
}

utfChar::~utfChar(){
	delete [] data;
}

//using linux/windows wchat_t, no need to check if its a valid unicode I dont think
utfChar::utfChar( wchar_t c ){
	data = new unsigned char[sizeof( wchar_t)];
	
	for( unsigned int i = 0; i < sizeof(wchar_t); i++ ){
		data[i] = c >> ( 8 * i ) & 0xFF;
	}
}

utfChar::utfChar( unsigned char c ){
	data = new unsigned char[1];

	//10000000 & 10000 which is invalid for single size
	if( c & 0x80 ){
		_error = UTF_INVALID;
		data[0] = 0;
		return;
	}

	data[0] = c;
}

utfChar::utfChar( unsigned char* c ){
	char size = sizeof(c);
	data = new unsigned char[size];

	//make sure the data doesn start with 10
	if( c[0] & 0xD0 == 0x80 ){
		_error = UTF_INVALID;
		delete [] data;
		data = 0;
		return;
	}

	for( int i = 0; i < size; i++ ){
		data[i] = c[i];
	}
}

utfChar::utfChar( unsigned short c ){
	data = new unsigned char[ sizeof( unsigned short ) ];
}
*/
