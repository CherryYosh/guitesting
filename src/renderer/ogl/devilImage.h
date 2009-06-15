/* 
 * File:   devilImage.h
 * Author: brandon
 *
 * Created on May 21, 2009, 8:26 PM
 */

#ifndef _DEVILIMAGE_H
#define	_DEVILIMAGE_H

#include "../image.h"

#include <string>

class devilImage : public Image {
public:
	static bool InitDevIL();

	devilImage();
	devilImage(const char*);
	devilImage(std::string);
	devilImage(const devilImage& orig);
	virtual ~devilImage();

	void Load(std::string);
	unsigned int GetID();
	unsigned int Width();
	unsigned int Height();
	unsigned int Format();
	unsigned int BPP();

private:
	unsigned int id;
	unsigned int width;
	unsigned int height;
	unsigned int format;
	unsigned int bpp;
};

#endif	/* _DEVILIMAGE_H */

