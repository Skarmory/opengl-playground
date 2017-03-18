#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 view_pos;

in vec3 Normal;
in vec3 Fragment_Position;
in vec3 Light_Position;
in vec2 TexCoords;

out vec4 colour;

void main()
{
	// Ambient
	vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
	
	// Diffuse
	vec3 norm = normalize(Normal);
	//vec3 light_direction = normalize(Light_Position - Fragment_Position);
	vec3 light_direction = normalize(-light.direction);
	float diff = max(dot(norm, light_direction), 0.0f);
	vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;
	
	// Specular
	vec3 view_direction = normalize(view_pos - Fragment_Position);
	vec3 reflect_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), 32);
	vec3 specular = spec * vec3(texture(material.specular, TexCoords)) * light.specular;
	
	vec3 emission = vec3(texture(material.emission, TexCoords));
	
	colour = vec4((ambient + diffuse + specular + emission), 1.0f);
}