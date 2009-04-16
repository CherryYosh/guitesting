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

in vec2 vertex;
in vec2 tcoord;
in vec4 tcolor;

centroid out vec2 texCoord;
centroid out vec4 texColor;

void main(){
	texCoord = tcoord;
	texColor = tcolor;
	gl_Position = projection * modelview * vec4(vertex, 0.0, 1.0 );
}
