#ifndef THEMEMGR_H
#define THEMEMGR_H

#include <string>
#include <vector>

//keep to the file
struct ThemeMgr_ImageDataT{	
	unsigned int x; //the left side x
	unsigned int y; //the top side y;
	unsigned int x2; //the right x
	unsigned int y2; //the bottom y

	std::string type;
};

//keep this to file scope
struct ThemeMgr_ThemeDataT{
	unsigned int imageID;
	unsigned int width;
	unsigned int height;
	std::string Name; //needed?

	std::vector< ThemeMgr_ImageDataT* > data; //maybe replace this later, but im feeling lazy right now
};

unsigned int ThemeMgr_LoadTheme( const char* );
unsigned int ThemeMgr_GetImage();
ThemeMgr_ThemeDataT* ThemeMgr_GetTheme();

#endif
