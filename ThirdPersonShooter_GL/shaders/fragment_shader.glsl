#version 400 core
in vec3 v_normal;
in vec3 frag_position;
in vec2 tex_coord;
in vec3 frag_normal;
in mat3 tbn_mat;
out vec4 frag_color;
uniform vec3 camera_position;
uniform vec3 light;
uniform mat4 light_mat;
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_map;

void main()
{
	vec3 frag_normal = texture(normal_map, tex_coord).xyz;
	frag_normal = normalize(frag_normal * 2.0f - 1.0f);
	frag_normal = normalize(tbn_mat * frag_normal);

	vec3 direction_to_light = normalize(light - frag_position);
	vec3 direction_to_camera = normalize(camera_position - frag_position);

	float ambient_intensity = 0.7f;
	vec4 ambient_color =  texture(diffuse_texture, tex_coord) * ambient_intensity;
	
	float diffuse_intensity = dot(frag_normal, direction_to_light);
	vec4 diffuse_color = texture(diffuse_texture, tex_coord) * diffuse_intensity; 

	vec3 reflected_ray = normalize(reflect(-direction_to_light, frag_normal));
	float specular_intensity = pow(max(dot(direction_to_camera, reflected_ray), 0.0), 16.0f);
	vec4 specular_color = vec4(1.0f, 1.0f, 1.0f, 1.0f) * texture(specular_texture, tex_coord) * specular_intensity;

	frag_color = ambient_color + diffuse_color + specular_color;
}
