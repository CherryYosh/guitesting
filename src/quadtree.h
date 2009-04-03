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
