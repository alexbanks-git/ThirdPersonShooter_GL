#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texture_coordinate;
layout (location = 5) in uvec4 bone_ids;
layout (location = 6) in vec4 bone_weights;

out vec3 v_tangent;
out vec3 v_bitangent;
out vec3 v_position;
out vec2 tex_coord;
out vec3 v_normal;
out vec3 frag_position;
out mat3 tbn_mat;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bone_mats[100];

void main()
{
	vec4 final_position = vec4(0);
	for (int i = 0; i < 4; i++)
	{
		final_position += bone_mats[bone_ids[i]] * vec4(position, 1.0f) * bone_weights[i];
	}

	tex_coord = texture_coordinate;

	v_tangent = normalize(mat3(transpose(inverse(model))) * tangent);
	v_bitangent = normalize(mat3(transpose(inverse(model))) * bitangent);
	v_normal = normalize(mat3(transpose(inverse(model))) * normal);

	tbn_mat = mat3(v_tangent, v_bitangent, v_normal);

	frag_position = vec3(model * final_position);
	gl_Position = projection * view * model * final_position;
}
