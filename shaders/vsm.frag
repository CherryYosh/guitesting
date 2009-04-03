//
//
//
//
//
#version 120
uniform sampler2D shadowMap;

varying vec4 shadowCoord;

vec2 depth;
vec4 sc;

float chebyshevUpperBound( float dist ){
	depth = texture2D( shadowMap, sc.st).rg;

	if( dist <= depth.x  )
		return 1.0;

	float variance = depth.y - ( depth.x*depth.x );
	variance = max(variance,0.00002);
	
	float d = dist - depth.x;
	float p_max = variance / ( variance + d*d );

	return p_max;
}

void main(){
	sc = shadowCoord / shadowCoord.w;
	float shadow = chebyshevUpperBound( sc.z );

	gl_FragData[0] = vec4(shadow) * gl_Color;
}
