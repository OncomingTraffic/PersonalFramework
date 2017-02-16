attribute vec4 a_Position;
attribute vec4 a_Normal;
attribute vec2 a_UV;



// Point light info
uniform struct PointLight
{
	vec3 Pos;
	vec3 Color;
	//brightness, attenuation, attenuation coefficient
};

uniform PointLight u_Light[4];

// Directional light info (support only 1 light?)
struct DirectionalLight
{
	vec3 Direction;
	vec3 Color;
};

uniform DirectionalLight u_DirectionalLight;

uniform vec3 u_CameraPos;


uniform mat4 u_WVP;
uniform mat4 u_ModelMatrix;
uniform vec4 u_AmbientLight = vec4(1, 0.5f, 0.5f, 1);



varying vec4 v_VertexPos;
varying vec3 v_VertexNormal;
varying vec2 v_UV;



void main()
{

	v_VertexPos = u_ModelMatrix * a_Position;
	gl_Position = u_WVP * vec4( a_Position.xyz, 1 );

	v_VertexNormal = a_Normal;

	v_UV = a_UV;
}