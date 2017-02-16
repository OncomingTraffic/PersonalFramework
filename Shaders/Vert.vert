#version 140

attribute vec2 a_Position;

void main()
{
	gl_Position = vec4( a_Position.x, a_Position.y, 0, 1 );
}