#ifndef RULE_H
#define RULE_H

#include "control.h"

class Rule : public Control {
public:
	Rule( Window* p, std::string t, int x, int y );
	~Rule();

	bool HitTest( int mouseX, int mouseY );
protected:
private:
};

#endif
