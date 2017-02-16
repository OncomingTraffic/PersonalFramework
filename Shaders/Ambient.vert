attribute vec3 a_Position;
attribute vec3 a_Normal;

uniform mat4 u_WVP;
uniform vec4 u_AmbientLight = vec4(1, 0.5, 0.5, 1);

varying vec3 v_Normal;

void main()
{
	gl_Position = u_WVP * vec4( a_Position, 1 );

	v_Normal = a_Normal;
}