varying vec4 vPos;

void main(){
	float depth = vPos.z / vPos.w;
	depth = depth * 0.5 + 0.5;

	float depth2 = depth * depth;
	float dx = dFdx(depth); 
	float dy = dFdy(depth); 
	depth2 = depth2 + 0.25*(dx*dx+dy*dy);

	gl_FragData[0] = vec4( depth, depth2, 0.0, 0.0 );
}
