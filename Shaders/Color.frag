
uniform vec4 u_Color;

varying vec2 v_UV;

void main()
{
	vec4 color = u_Color;
	
	float alpha = color.a;
	color.a += (v_UV.x < 0.01f) + (v_UV.x > 0.99f);
	color.a += (v_UV.y < 0.01f) + (v_UV.y > 0.99f);

	gl_FragColor = color;
	//LFragment = vec4( 1.0, 0.0, 1.0, 1.0 );
}
