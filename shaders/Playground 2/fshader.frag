#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct PointLight {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};
#define POINT_LIGHT_COUNT 4
uniform PointLight point_lights[POINT_LIGHT_COUNT];

struct SpotLight {
	vec3 position;
	vec3 direction;
	float inner_cutoff;
	float outer_cutoff;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};
uniform SpotLight spot_light;

struct DirectionalLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirectionalLight directional_light;

uniform Material material;
//uniform Light light;
uniform vec3 view_position;

in vec3 Normal;
in vec3 Fragment_Position;
in vec2 TexCoords;

out vec4 colour;

vec3 calculate_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_direction);
vec3 calculate_spot_light(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_direction);
vec3 calculate_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction);

void main()
{
	vec3 normal = normalize(Normal);
	vec3 view_direction = normalize(view_position - Fragment_Position);

	vec3 result = calculate_directional_light(directional_light, normal, view_direction);
	
	for(int i = 0; i < POINT_LIGHT_COUNT; i++)
		result += calculate_point_light(point_lights[i], normal, Fragment_Position, view_direction);
	
	result += calculate_spot_light(spot_light, normal, Fragment_Position, view_direction);
	
	colour = vec4(result, 1.0f); 
}

vec3 calculate_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_direction)
{
	vec3 light_dir = normalize(light.position - frag_pos);
	
	float diff = max(dot(normal, light_dir), 0.0f);
	
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_direction, reflect_dir), 0.0f), material.shininess);
	
	float distance = length(light.position - frag_pos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec3 calculate_spot_light(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_direction)
{
	// Set intensity based on whether inside cone
	vec3 light_dir = normalize(light.position - frag_pos);
	float theta = dot(light_dir, normalize(-light.direction));
	float epsilon = light.inner_cutoff - light.outer_cutoff;
	float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0f, 1.0f);
	
	float diff = max(dot(normal, light_dir), 0.0);
	
	vec3 reflect_direction = reflect(-light_dir, normal);  
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
	
	float distance    = length(light.position - frag_pos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	ambient  *= attenuation;  
	diffuse  *= attenuation * intensity;
	specular *= attenuation * intensity;   
			
	return (ambient + diffuse + specular); 
}

vec3 calculate_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction)
{
	vec3 light_dir = normalize(-light.direction);
	
	float diff = max(dot(normal, light_dir), 0.0f);
	
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_direction, reflect_dir), 0.0f), material.shininess);
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	return (ambient + diffuse + specular);
}