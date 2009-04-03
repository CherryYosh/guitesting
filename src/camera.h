#ifndef CAMERA_H
#define CAMERA_H

#include "nvMatrix.h"
#include "nvVector.h"

class Camera{
public:
	Camera();
	~Camera();

	void SetProjection( float fovy, float aspecty, float near, float far );
	void SetOrtho(float, float, float, float, float, float);
	void Move( float x, float y, float z );
	void Rotate( float x, float y, float z );
	
	float* GetProjection();
	float* GetOrtho();
	float* GetModelview();
	
	//these need to be public till I get off my ass :P
	float fov, aspect, zNear, zFar;
private:
	nv::matrix4<float> projection;
	nv::matrix4<float> ortho;
	nv::matrix4<float> modelview;

};

#endif

