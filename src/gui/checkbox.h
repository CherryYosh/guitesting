/* 
 * File:   checkbox.h
 * Author: brandon
 *
 * Created on May 2, 2009, 5:23 PM
 */

#ifndef CHECKBOX_H
#define	CHECKBOX_H

#include "control.h"

class Checkbox : public Control {
public:
	Checkbox(std::string, Window*, Control* = NULL, float = 0, float = 0);
	virtual ~Checkbox();

	bool OnMouseClick(unsigned short, bool);


private:
	bool isChecked;
};

#endif	/* _CHECKBOX_H */

