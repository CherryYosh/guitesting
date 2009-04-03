#ifndef LIGHT_H
#define LIGHT_H

#include <GL/gl.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "nvMatrix.h"
#include "nvVector.h"

class Light{
public:
	nv::vec3<double> center;
	nv::vec3<double> viewDirection;
	
	//near and far frustum distances
	double near;
	double far;

	//width, height, and ratio of the shadowmap
	float width;
	float height;
	double ratio;

	nv::matrix4<double> modelView;
	nv::matrix4<double> projection;
	nv::matrix4<double> projMV; //projection * modelView


	float color[3];
	
	bool isEnabled;
	int type;		// 0 = disabled 1 = point TODO
	int shadowType;		//0 = none 1= VSM 2 = CSM 3 = CVSM TODO: FIND A BETTER WAY 		
	GLuint shadowDepthMap;	//is this really needed?? 
	GLuint shadowColorMap;
	GLuint shadowBlurMap;	//only needed for VSM... gah so much fill -_-
	bool usingShadows;

	Light();
	void Move( double x, double y, double z ); //NOTE: first time this will effectivly set the position
	void Rotate( double x, double y, double z );
	void SetColor( float r, float g, float b );
	void SetType( int _type );
	bool UpdateShadowMaps();
};

#endif
