#include "thememgr.h"

#include "image.h"

#include <stdlib.h>

//for files
#include <iostream>
#include <fstream>

using namespace std;

ThemeMgr_ThemeDataT theme; //needs to be visible to the gui

//============== theme data is store like
// image:default.png 
// close:toprightX, toprightY, width, height, more!

//MUST BE CALLED AFTER DEVIL_INIT
unsigned int ThemeMgr_LoadTheme( const char* themename ){
	string line; //the current line
	string temp; //temp data, a substring of line
	size_t  pos; //position in the search
	size_t pos2; //the second pos
	unsigned int count = 0;


	ifstream file;
	file.open( themename );

	if( !file.is_open() ){
		return 0;
	}

	while( !file.eof() ){
		getline( file, line );

		//first we get the type or first word
		pos = line.find( ':' );

		if( pos == string::npos )
			return count;
		
		temp = line.substr( 0, pos ); //get from start to pos
		
		if( temp.compare( "image" ) == 0 ){
			temp = line.substr( pos+1 ); //now we get the rest for the image name, add 1 to remove the :
			temp.insert( 0, "themes/" ); //insert the code path, change to soft code??
			theme.imageID = GetGLTexture( temp.c_str(), &theme.width, &theme.height );	
			continue;
		} else { //the data is pretty simple and uniform right now..
			ThemeMgr_ImageDataT* data = new ThemeMgr_ImageDataT;
			data->type = temp;

			//get x1
			pos2 = line.find( ",", pos+1 ); //find the first ','
			temp = line.substr( pos+1, pos2 - pos - 1 ); //remove the : and get the string till the ,
			data->x = atoi( temp.c_str() );

			//get y1, pos and pos2 are swaped out to keep from having to swap them
			pos = line.find( ",", pos2+1 );
			temp = line.substr( pos2+1, pos - pos2 - 1 );
			data->y = atoi( temp.c_str() );
			
			//get x2
			pos2 = line.find( ",", pos+1 ); //find the first ','
			temp = line.substr( pos+1, pos2 - pos - 1 ); //remove the : and get the string till the ,
			data->x2 = atoi( temp.c_str() );
			
			//get y2
			pos = line.find( ",", pos2+1 );
			temp = line.substr( pos2+1, pos - pos2 - 1 );
			data->y2 = atoi( temp.c_str() );
			
			theme.data.push_back( data );
			
			//keep count so we can get the size of the bufferes needed
			count++;
		}
	}
	
	return count;
}

unsigned int ThemeMgr_GetImage(){
	return theme.imageID;
}

//WARNING!!!!! this gives you a pointer to the data!
ThemeMgr_ThemeDataT* ThemeMgr_GetTheme(){
	return &theme;
}
