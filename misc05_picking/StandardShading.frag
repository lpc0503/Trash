#version 330 core

// Interpolated values from the vertex shaders
in vec4 vs_vertexColor;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;

struct DirectionalLight
{
	vec3 dir;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirectionalLight dirLight;

uniform bool uEnableLight;

uniform vec4 Color;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal)
{
	vec3 lightDir = (MV * vec4(-light.dir, 0.0)).xyz;
	lightDir = normalize(lightDir);
//	vec3 lightDir = normalize(-light.dir);
//	lightDir = (MV * vec4(lightDir, 0.0)).xyz;
	//
	float diffuse = max(dot(lightDir, normal), 0.0);
	vec3 diffuseColor = light.diffuse * diffuse;
	//
//	float specular = pow(max(dot(reflect(-lightDir, normal), viewDir), 0.0), 32.0);
//	vec3 specularColor = light.specular * specular;
	//
	return light.ambient * 0.1 + diffuseColor * 0.1 * diffuseColor;
}

void main()
{
	if(uEnableLight)
	{
		color = Color.rgb * CalcDirectionalLight(dirLight, Normal_cameraspace);
	}
	else
	{
		color = vs_vertexColor.rgb;
	}
}