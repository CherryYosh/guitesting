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
#ifndef QUADTREE_H
#define QUADTREE_H

#define _MINOBJECTS 1 //if there are fewer objects then this we will combine two objects
#define _MAXOBJECTS 5 //force a new node if there are more then X objects
#define _MINWIDTH 5 //the smallest a node will get
#define _MINHEIGHT 5 


struct Node{
	bool isLeaf; //does it have children
	Node* children[4];
	Node* parent;

	union{
		float points[8];
		float topLeft[2], topRight[2], bottomRight[2], bottomLeft[2];
	};

	float width, height;

	unsigned int id;

	//std::vector<void*> objects;
}* qt_root;

void quadtree_Init( float tx, float ty, float bx, float by);
void quadtree_Update();
void quadtree_AddObject(); //how to do this..

#endif
