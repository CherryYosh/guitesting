#include "light.h"

Light::Light(){
	//fill center, viewDirection, and color with 0
	memset( center, 0, sizeof(nv::vec3<double>) );
	memset( viewDirection, 0, sizeof(nv::vec3<double>) );
	memset( color, 0, 3*sizeof(float) );
	
	near = 1.0;
	far = 100.0;

	width = 512.0;
	height = 512.0;
	ratio = width / height;

	modelView.make_identity();
	projection.make_identity();
	projMV.make_identity();

	isEnabled = true;
	type = 1;
	shadowType = 1;
	shadowDepthMap = 0; //NOTE: needs to be set up!!!
	shadowColorMap = 0;
	shadowBlurMap = 0;
	usingShadows = false;
}

void Light::Move( double x, double y, double z ) {
	center[0] += x;
	center[1] += y;
	center[2] += z;
}

void Light::Rotate( double x, double y, double z ){
	viewDirection[0] += x;
	viewDirection[1] += y;
	viewDirection[2] += z;
}

void Light::SetColor( float r, float g, float b ){
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void Light::SetType( int _type ){
	type = _type;
}

//updates / creates the shadowmaps
//TODO: Better debuging!
bool Light::UpdateShadowMaps(){
	if( usingShadows == false )//should we delete them here??
		return false;
	
	if( shadowDepthMap == 0 )
		glGenTextures( 1, &shadowDepthMap );
	if( shadowColorMap == 0 )
		glGenTextures( 1, &shadowColorMap );
	if( shadowBlurMap == 0 )
		glGenTextures( 1, &shadowBlurMap );
	
	//============================================ setup the depth map
	glBindTexture( GL_TEXTURE_2D, shadowDepthMap );
	//no filtering, not sure if its possible
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//tell it to clamp to the edge.. prevents a few bugs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//and finally we create the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//============================================ setup the color map
	glBindTexture( GL_TEXTURE_2D, shadowColorMap );
	//no filtering, not sure if its possible
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//tell it to clamp to the edge.. prevents a few bugs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//and finally we create the texture, RGB so that we save a little space
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F_ARB, width, height, 0, GL_RGB, GL_FLOAT, NULL);

        //============================================ setup the blur map
        glBindTexture( GL_TEXTURE_2D, shadowBlurMap );
        //no filtering, not sure if its possible
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //tell it to clamp to the edge.. prevents a few bugs
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //and finally we create the texture, RGB so that we save a little space
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, NULL);	
	
	//unbind the texture to prevent bugs
	glBindTexture( GL_TEXTURE_2D, 0 );

	GLenum status = glGetError();
	if( status != GL_NO_ERROR )
		return false;
	return true;
}
