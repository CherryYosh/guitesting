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
#ifndef CAMERA_H
#define CAMERA_H

#include "utils/matrix.h"
#include "utils/vector.h"

class Camera{
public:
	Camera();
	~Camera();

	void SetProjection( float fovy, float aspecty, float near, float far );
	void SetOrtho(float, float, float, float, float, float);
	void Move( float x, float y, float z );
	void Rotate( float x, float y, float z );
	
	float* GetProjectionfv();
	float* GetOrthofv();
	float* GetModelviewfv();

	util::matrix4<float>* GetProjection();
	util::matrix4<float>* GetOrtho();
	util::matrix4<float>* GetModelview();
	
	//these need to be public till I get off my ass :P
	float fov, aspect, zNear, zFar;
private:
	util::matrix4<float> projection;
	util::matrix4<float> ortho;
	util::matrix4<float> modelview;
};

#endif

