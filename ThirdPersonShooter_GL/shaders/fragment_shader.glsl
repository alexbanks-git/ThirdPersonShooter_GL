#version 400 core
in vec3 v_normal;
in vec3 frag_position;
in vec2 tex_coord;
in mat3 tbn_mat;
in vec4 light_pos;
out vec4 frag_color;

uniform vec3 camera_position;
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_map;
uniform sampler2D depth_map;
uniform int light_count;

struct PointLight
{
	vec3 position;
	float intensity;
	float radius;
	float att_constant;
	float att_linear;
	float att_quadratic;
};

struct DirectionalLight
{
	vec3 direction;
	float intensity;
};

uniform PointLight point_lights[20];
uniform DirectionalLight directional_light;

void main()
{
	vec3 proj = vec3(light_pos * 0.5f + 0.5f);
	float closest = texture(depth_map, proj.xy).r;
	float current = proj.z;
	

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

	if (textureSize(normal_map, 0).x == 1)
	{
		frag_normal = v_normal;
	}

	vec3 direction_to_light = vec3(0.0f);
	vec3 direction_to_camera = normalize(camera_position - frag_position);

	float ambient_intensity = 0.2f;
	vec4 ambient_color = diffuse * ambient_intensity;
	
	float diffuse_intensity = 0.0f;
	vec4 diffuse_color = vec4(0.0f);

	vec3 reflected_ray = vec3(0.0f);
	float specular_intensity = 0.0f;
	vec4 specular_color = vec4(0.0f);
	float bias = 0.002f;
	
	for (int l = 0; l < light_count; l++)
	{
		float distance = length(frag_position - point_lights[l].position);
		float attenuation = 1.0f / (point_lights[l].att_constant + point_lights[l].att_linear * distance + point_lights[l].att_quadratic * pow(distance, 2));
		direction_to_light = normalize(point_lights[l].position - frag_position);
		diffuse_intensity += max(dot(frag_normal, direction_to_light), 0.0f);
		diffuse_color += diffuse * diffuse_intensity * point_lights[l].intensity * vec4(1.0f);

		reflected_ray = normalize(reflect(-direction_to_light, frag_normal));
		specular_intensity += pow(max(dot(direction_to_camera, reflected_ray), 0.0), 16.0f);
		specular_color += specular * specular_intensity * point_lights[l].intensity * vec4(1.0f);

		diffuse_color *= attenuation;
		specular_color *= attenuation;
		ambient_color *= attenuation;
	}

	vec3 light_direction = -directional_light.direction;
	diffuse_intensity += max(dot(frag_normal, light_direction), 0.0f);
	diffuse_color += diffuse * diffuse_intensity * directional_light.intensity * vec4(1.0f);

	reflected_ray = normalize(reflect(-light_direction, frag_normal));
	specular_intensity += pow(max(dot(direction_to_camera, reflected_ray), 0.0), 16.0f);
	specular_color += specular * specular_intensity * directional_light.intensity * vec4(1.0f);

	float shadow = current - bias > closest  ? 1.0 : 0.0;

	if (proj.z > 1.0f)
		shadow = 0.0f;

	shadow = (1.0f - (shadow * 0.5f));

	vec4 color = (ambient_color + shadow * (diffuse_color + specular_color));

	frag_color = color;
}
