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
#include "camera.h"
#include <math.h>

Camera::Camera(){
	modelview.make_identity();
	projection.make_identity();
	ortho.make_identity();
	//NOTE: make sure you set the projection!!

	fov = 45.0;
	aspect = 1.0;
	zNear = 1.0;
	zFar = 100.0;
}

Camera::~Camera(){
	//TODO
}

void Camera::Move( float x, float y, float z ){
	modelview._41 += x;
	modelview._42 += y;
	modelview._43 += z;
}

void Camera::Rotate( float x, float y, float z ){
}

void Camera::SetProjection( float fovy, float aspecty, float near, float far ){
	fov = fovy;
	aspect = aspecty;
	zNear = near;
	zFar = far;

	float f = 1.0 / tan( fovy / 2.0 );
	//set up the scale
	//http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
	nv::vec3<float> scale = nv::vec3<float>( 	f / aspect, 
							f, 
							( zFar + zNear ) / (zNear - zFar )  );
	
	//sets the values of [i,i] = 1 other wise all are 0
	projection.make_identity();
	//set the projection data
	projection.set_scale( scale );
	projection._34 = -1.0;
	projection._43 = ( 2.0 * zFar * zNear ) / ( zNear - zFar );
	projection._44 =  0.0;
}

void Camera::SetOrtho( float left, float right, float bottom, float top, float near, float far ){
	nv::vec3<float> scale = nv::vec3<float>( 	2.0 / ( right - left ),
							2.0 / ( top - bottom ),
							-2.0 / ( far - near ) );
	nv::vec3<float> trans = nv::vec3<float>(	-( right + left ) / ( right - left ),
							-( top + bottom ) / ( top - bottom ),
							-( far + near ) / ( far - near ) );

	ortho.make_identity();
	ortho.set_scale( scale );
	ortho.set_translate( trans );
}

float* Camera::GetProjection(){
	return projection._array;
}

float* Camera::GetOrtho(){
	return ortho._array;
}

float* Camera::GetModelview(){
	return modelview._array;
}
