//
//
//
//
//
#version 130

precision mediump float;

uniform sampler2D tex0;

centroid in vec2 texCoord;

out vec4 pixelColor;

void main(){
	pixelColor = texture( tex0, texCoord );
	//pixelColor = vec4( texCoord, 0.0, 1.0 );
	//pixelColor = vec4(1.0,0.0,0.0,1.0 );
}
