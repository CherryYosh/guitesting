#version 130

//this reall does nothing, according to the API
precision highp float;

uniform sampler2D tex0;

in vec2 texCoord;
in vec4 color;

out vec4 pixelColor;

void main(){
	pixelColor = color * texture(tex0, texCoord);
}
