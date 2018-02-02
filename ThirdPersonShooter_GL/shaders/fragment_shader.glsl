#version 400 core
in vec3 v_normal;
in vec3 frag_position;
in vec2 tex_coord;
in mat3 tbn_mat;
out vec4 frag_color;

uniform vec3 camera_position;
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_map;
uniform int light_count;

struct Light
{
	vec3 position;
	float intensity;
	float radius;
};

uniform Light lights[20];

void main()
{
	vec3 frag_normal = texture(normal_map, tex_coord).xyz;
	vec4 diffuse = vec4(0);
	vec4 specular = vec4(0);

	if (textureSize(diffuse_texture, 0).x == 1)
	{
		diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		diffuse = texture(diffuse_texture, tex_coord);
	}

	if (textureSize(specular_texture, 0).x == 1)
	{
		specular = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		specular = texture(specular_texture, tex_coord);
	}

	frag_normal = normalize(frag_normal * 2.0f - 1.0f);
	frag_normal = normalize(tbn_mat * frag_normal);

	vec3 direction_to_light = vec3(0.0f);
	vec3 direction_to_camera = normalize(camera_position - frag_position);

	float ambient_intensity = 0.7f;
	vec4 ambient_color = diffuse * ambient_intensity;
	
	float diffuse_intensity = 0.0f;
	vec4 diffuse_color = vec4(0.0f);

	vec3 reflected_ray = vec3(0.0f);
	float specular_intensity = 0.0f;
	vec4 specular_color = vec4(0.0f);

	for (int l = 0; l < light_count; l++)
	{
		direction_to_light = normalize(lights[l].position - frag_position);
		diffuse_intensity += dot(frag_normal, direction_to_light);
		diffuse_color += diffuse * diffuse_intensity * lights[l].intensity;

		reflected_ray = normalize(reflect(-direction_to_light, frag_normal));
		specular_intensity += pow(max(dot(direction_to_camera, reflected_ray), 0.0), 16.0f);
		specular_color += vec4(1.0f, 1.0f, 1.0f, 1.0f) * specular * specular_intensity * lights[l].intensity;
	}

	frag_color = ambient_color + diffuse_color + specular_color;
}
