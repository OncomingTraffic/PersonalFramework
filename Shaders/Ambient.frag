uniform vec4 u_AmbientLight = vec4(0.5, 0.5, 0.5, 1);

varying vec3 v_Normal;

void main()
{
	vec4 color = vec4(u_AmbientLight.xyz * dot(v_Normal, vec3(0, 0, -1)), 1);

	gl_FragColor = color;
	//gl_FragColor = u_AmbientLight;
	
}
