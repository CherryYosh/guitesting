//James Brandon Stevenson
//
//
//
//
#version 130

//this is needed to fix ATI bugs
precision mediump float;

uniform mat4 projection;
uniform mat4 modelview;
in vec2 vertex;

in vec2  tcoord;

centroid out vec2 texCoord;

void main(){
	texCoord = tcoord; //interpolate the coords

	gl_Position = projection * modelview * vec4(vertex,0.0,1.0);
}
