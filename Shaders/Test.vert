attribute vec3 a_Position;

uniform mat4 u_WVP;

varying vec4 v_Color;

void main()
{
	gl_Position = u_WVP * vec4( a_Position, 1 );

	v_Color = vec4(1, 1, 1, 1);
}