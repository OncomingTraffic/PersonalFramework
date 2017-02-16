
varying vec4 v_Color;

void main()
{
	vec4 color = v_Color;
	color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//color = vec4(0, 1.0f, 1.0f, 0);
	gl_FragColor = color;
}
