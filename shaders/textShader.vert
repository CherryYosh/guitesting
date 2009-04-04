//James Brandon Stevensn
//a simple text shader
//more to come!
//
//
#version 130

//fix a ati bug
precision mediump float;

uniform mat4 projection;
uniform mat4 modelview;

in vec4 vertex;
in vec2 tcoord;

centroid out vec2 texCoord;

void main(){
	texCoord = tcoord;
	gl_Position = projection * modelview * vertex;//vec4( vec3(vertex), 1.0 );
}
