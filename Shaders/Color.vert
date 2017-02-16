attribute vec3 a_Position;
attribute vec2 a_UV;

uniform mat4 u_WVP;

uniform vec4 u_Color;

varying vec2 v_UV;

void main()
{
	gl_Position = u_WVP * vec4( a_Position, 1 );
	//gl_Position = vec4( a_Position.x / 50, a_Position.y / 50, a_Position.z / 50, 1 );

	v_UV = a_UV;
}