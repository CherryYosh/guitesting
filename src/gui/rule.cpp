//A base rule class used for things like borders, spacers seperators etc


#include "rule.h"

Rule::Rule( std::string t, int x, int y ) : Control(t,x,y) {
}

Rule::~Rule(){
}

bool Rule::HitTest( int mouseX, int mouseY ){
	if( mouseX > x && mouseX < x + width &&
		mouseY > y && mouseY < y + height ){
		return true;
	}
	return false;
}
