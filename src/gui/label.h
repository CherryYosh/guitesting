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
#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "control.h"
#include "../colorstring.h"

#include <string>
#include <vector>
#include <list>


class Label : public Control {
public:
    Label(long, Window* = NULL, Control* = NULL, LayerT = DEFAULT_LAYER, float = 0, float = 0);
	Label(Window* = NULL, Control* = NULL, LayerT = DEFAULT_LAYER, float = 0, float = 0);
    Label(const Label&);
	Label(const Control&);
	~Label();

        Control* clone();

        virtual void AddString(colorstring);
	virtual void AddString(std::string , util::Color = "#FFFFFF");
        std::vector<colorstring>& GetText();

        void multiline(bool);
	bool multiline();
protected:
	bool isMultiline;
	std::vector<colorstring> text;
private:
};

#endif
