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
	
	vec4 color = texture( tex0, texCoord );
	if( color.a == 0.0 || color.rgb == vec3( 0.0 ) )
		discard;
	pixelColor = color;
}
