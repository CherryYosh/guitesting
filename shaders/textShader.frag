//
//
//
//
//
#version 130

precision mediump float;

uniform sampler2D tex0;

centroid in vec4 texColor;
centroid in vec2 texCoord;

out vec4 pixelColor;

void main(){
	
	vec4 color = texture( tex0, texCoord );
	pixelColor =  texColor * color;
}
