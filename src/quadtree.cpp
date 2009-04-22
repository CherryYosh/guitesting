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
