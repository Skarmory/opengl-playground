#version 330 core

uniform vec3 object_colour;
uniform vec3 light_colour;

in vec3 Normal;
in vec3 Fragment_Position;
in vec3 Light_Position;

out vec4 frag_colour;

void main()
{
	float ambient_strength = 0.1f;
	vec3 ambient = ambient_strength * light_colour;
	
	vec3 norm = normalize(Normal);
	vec3 light_direction = normalize(Light_Position - Fragment_Position);
	
	float diff = max(dot(norm, light_direction), 0.0f);
	vec3 diffuse = diff * light_colour;
	
	float specular_strength = 0.5f;
	vec3 view_direction = normalize(-Fragment_Position);
	vec3 reflect_direction = reflect(-light_direction, norm);
	
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), 32);
	vec3 specular = specular_strength * spec * light_colour;
	
	frag_colour = vec4((ambient + diffuse + specular) * object_colour, 1.0f);
}