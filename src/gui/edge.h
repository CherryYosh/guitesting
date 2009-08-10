/* 
 * File:   edge.h
 * Author: brandon
 *
 * Created on July 12, 2009, 7:03 PM
 */

#ifndef _EDGE_H
#define	_EDGE_H

#include "Widget.h"

class Edge : public Widget{
public:
    Edge();
    Edge(const Edge& orig);
    virtual ~Edge();

    Widget* clone();

    void OnMousePress(unsigned short, int, int);
    bool OnMouseMotion(float, float, unsigned short);

private:
    float oldx, oldy;
};

#endif	/* _EDGE_H */

