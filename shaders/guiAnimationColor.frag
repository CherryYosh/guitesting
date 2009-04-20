#version 130

//this reall does nothing, according to the API
//need for ES compatiblity, and to fix a odd (and annoyign) bug in ATI
precision mediump float;

uniform sampler2D tex0;
uniform vec4 color;

centroid in vec2 texCoord;

out vec4 pixelColor;

void main(){
	pixelColor = color + texture( tex0, texCoord );
}
