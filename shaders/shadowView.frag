//
//
//
//
//
#version 120

varying vec2 texCoord;

uniform sampler2D shadowmap;

void main(){
	vec3 c = texture2D( shadowmap, texCoord ).xyz;
	//x = pow(x, 32.0 );

	gl_FragData[0] = vec4( c, 1.0 );
}
