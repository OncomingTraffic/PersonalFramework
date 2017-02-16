uniform vec4 u_AmbientColor = vec4(0.5, 0.5f, 0.5, 1);

varying vec4 v_VertexPos;
varying vec3 v_VertexNormal;
varying vec2 v_UV;

vec4 surfaceColor = vec4(1, 1, 1, 1);

uniform vec3 u_CameraPos;

uniform sampler2D u_Texture;


const unsigned int MAX_LIGHTS = 4;


// Structs need to be defined in any file they're used in. (defining in the vert shader doesn't allow it to be used in the frag shader)
// The struct itself must be declared as a uniform to allow member variables to be set through code
uniform struct PointLight
{
	vec3 Pos;
	vec3 Color;
	vec3 Attenuation;
	//brightness, attenuation, attenuation coefficient
};
uniform PointLight u_Light[MAX_LIGHTS];

uniform struct DirectionalLight
{
	vec3 Direction;
	vec3 Color;
};

uniform DirectionalLight u_DirectionalLight;

vec3 CalculateLighting(); //TODO: Create a way to import shader data, similar to .cginc files

void main()
{

	vec4 color = vec4(CalculateLighting(), 1);
	//vec4 color = vec4(uColor * uAmbient, 1);

	gl_FragColor = color;
	
}


// To use this in the shader, it must be declared before any calls to it are made
vec3 CalculateLighting()
{
	vec4 color = texture2D( u_Texture, v_UV );

	vec3 finalColor = u_AmbientColor * color.rgb;

	// PointLights
	for (unsigned short i = 0; i < MAX_LIGHTS; ++i)
	{
		vec3 lightDirection = u_Light[i].Pos - v_VertexPos;
		float dist = length(lightDirection);
		lightDirection = normalize(lightDirection);

		vec3 viewDirection = normalize(u_CameraPos - v_VertexPos);

		float dotProduct = max(dot(v_VertexNormal, lightDirection), 0);

		// Add 1 to denominator to prevent divide by 0
		float atten = 1.0f / (1 + u_Light[i].Attenuation.x + u_Light[i].Attenuation.y * dist + u_Light[i].Attenuation.z * dist * dist);

		vec3 diffuse = u_Light[i].Color * color.rgb * max(0.0, dotProduct);

		float specPerc = max(0.0, dot(reflect(-lightDirection, v_VertexNormal), viewDirection));
			
		//vec3 specular = specColor * pow(specPerc, shininess);
		vec3 specular = vec3(1, 1, 1) * pow(specPerc, 80);

		finalColor += (diffuse + specular) * atten;
	}


	//Directional Light
	{
		vec3 lightDirection = normalize(u_DirectionalLight.Direction);

		vec3 viewDirection = normalize(u_CameraPos - v_VertexPos);

		float dotProduct = max(dot(v_VertexNormal, -lightDirection), 0);

		vec3 diffuse = u_DirectionalLight.Color * color.rgb * dotProduct;

		float specPerc = max(0, dot(reflect(-lightDirection, v_VertexNormal), viewDirection));
			
		//vec3 specular = specColor * pow(specPerc, shininess);
		vec3 specular = vec3(1, 1, 1) * pow(specPerc, 80);

		finalColor += (diffuse + specular);
	}

	return finalColor;
}

// Find the direction from the light to the vertex
// Clamp the dotProduct to 0 if it is less than zero

//Get dot product of light normal and vertex normal
//Multiply by attenuation
//Clamp01
//Multiply by light color
//Add ambient + diffuse + specular
//Multiply by surface color

//float3 lightDirection;

//float atten = unity_4LightAtten0[i]; //attenuation
		
// Get the world space position of the point light
//float3 lightPosition = float3(unity_4LightPosX0[i], unity_4LightPosY0[i], unity_4LightPosZ0[i]);

// Skip the point light if the light is at (0, 0, 0), as this is the default value
//if (length(lightPosition) != 0)

// Find the direction from the light to the vertex
//lightDirection = normalize(lightPosition - worldPosition);
			
// The dot product is equal to the Cos of the angle between the two vectors, so it will be between -1 and 1
// It is 1 if they are parallel, 0 if they are perpindicular, and -1 if the are parallel but in opposite directions
//float dotProduct = dot(normalDirection, lightDirection);
			
// Skip the lighting calculation if the surface normal is facing away from light direction
//if (dotProduct >= 0)
		
// Clamp the dotProduct to 0 if it is less than zero
//float3 diffuseReflection = unity_LightColor[i].rgb * color.rgb * max(0.0, dotProduct);
				

//Specular		
				
// Calculate the specular percentage based on the angle of the reflected light and the viewing angle
//float specPerc = max(0.0, dot(reflect(-lightDirection, normalDirection),  viewDirection));
		
//float3 specular = specColor * pow(specPerc, shininess);

//finalColor += (float4(diffuseReflection + specular, 1.0)) * atten;