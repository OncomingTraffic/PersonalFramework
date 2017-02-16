uniform sampler2D u_Texture;

varying vec2 v_UV;
varying vec4 v_Color;


void main()
{
	vec4 color = v_Color;
	//color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	color = texture2D( u_Texture, v_UV );

	//color = vec4(0, 1.0f, 1.0f, 1.0f);
	gl_FragColor = color;
	//LFragment = vec4( 1.0, 0.0, 1.0, 1.0 );
}
