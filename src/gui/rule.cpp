//A base rule class used for things like borders, spacers seperators etc


#include "rule.h"

Rule::Rule( Window* p, std::string t, int x, int y ) : Control(p,t,x,y) {
}

Rule::~Rule(){
}

bool Rule::HitTest( int mouseX, int mouseY ){
	if( mouseX > x && mouseX < x + Width &&
		mouseY > y && mouseY < y + Height ){
		return true;
	}
	return false;
}
