#version 120
uniform sampler2D tex;
uniform float width;
uniform float height;

varying vec2 texCoord;

//opengl does everything in 0~1, so convert the dimention into steps
float w = 1.0 / width;
float h = 1.0 / height;

void main(){
	vec4 color = vec4(0.0);

	color += texture2D( tex, texCoord + vec2(-w, h) );
	color += texture2D( tex, texCoord + vec2( 0, h) ) * 2.0;
	color += texture2D( tex, texCoord + vec2( w, h) );

	color += texture2D( tex, texCoord + vec2(-w, 0) ) * 2.0;
	color += texture2D( tex, texCoord + vec2( 0, 0) ) * 4.0;
	color += texture2D( tex, texCoord + vec2( w, 0) ) * 2.0;

	color += texture2D( tex, texCoord + vec2(-w,-h) );
	color += texture2D( tex, texCoord + vec2( 0,-h) ) * 2.0;
	color += texture2D( tex, texCoord + vec2( w,-h) );
	
	color = color / 16.0;
	
	gl_FragData[0] = color;
}

