//James Brandon Stevenson

#version 130

uniform mat4 projection;
uniform mat4 modelview;

in vec3 vertex;
in vec2 tcoord;
in vec4 tcolor;

out vec2 texCoord;
out vec4 color;

void main(){
	texCoord = tcoord;
	color = tcolor;

	gl_Position = projection * modelview * vec4(vertex,1.0);
}
