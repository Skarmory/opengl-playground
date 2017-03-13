#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 Fragment_Position;
in vec3 Light_Position;

out vec4 colour;

void main()
{
	// Ambient
	vec3 ambient = material.ambient * light.ambient;
	
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 light_direction = normalize(Light_Position - Fragment_Position);
	float diff = max(dot(norm, light_direction), 0.0f);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;
	
	// Specular
	vec3 view_direction = normalize(-Fragment_Position);
	vec3 reflect_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), 32);
	vec3 specular = material.specular * spec * light.specular;
	
	colour = vec4((ambient + diffuse + specular), 1.0f);
}