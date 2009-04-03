#include "quadtree.h"
#include <math.h>

//top left x, y, bottom right x, y
void quadtree_Init( float tx, float ty, float bx, float by ){
	qt_root->isLeaf = false;
	//qt_root->parent = NULL; //root will be the only one with this
	qt_root->id = 1;
	//qt_root->width = abs( tx ) + bx;
	//qt_root->height = abs( ty ) + by;

	//time to get the points
	//qt_root->topLeft = (float[]){ tx, ty };
}
