uniform vec4 u_AmbientLight = vec4(0.5, 0.5f, 0.5, 1);

varying vec3 v_Normal;


float atten = 1.0f;

vec3 uColor = vec3(1, 0, 1);

vec3 uAmbient = vec3(1, 1, 1);

vec3 uLightPos = vec3(0, 0, 0);


void main()
{
	vec4 color = vec4(uColor * uAmbient, 1);

	gl_FragColor = color;
	
}
