/* 
 * File:   edge.cpp
 * Author: brandon
 * 
 * Created on July 12, 2009, 7:03 PM
 */

#include "edge.h"
#include "window.h"

Edge::Edge() { }

Edge::Edge(const Edge& orig) { }

Edge::~Edge() { }

Widget* Edge::clone() {
    return new Edge(*this);
}

void Edge::OnMousePress(unsigned short button, int x, int y) {
    LockMouse();

    oldx = x;
    oldy = y;
}

bool Edge::OnMouseMotion(float lx, float ly, unsigned short button) {
    if (button == 1) { //TODO: Unhard-code
	//root->Resize(lx - oldx, ly - oldy);

	oldx = lx;
	oldy = ly;

	return true;
    }

    return false;
}