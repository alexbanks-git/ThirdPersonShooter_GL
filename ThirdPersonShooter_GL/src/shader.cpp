#include "shader.hpp"

static std::map<std::string, Shader> shaders;
static GLuint current_shader;

void Shader::init()
{
	shaders["image_shader"].id = create_shader_program("image_vertex_shader.glsl", "image_fragment_shader.glsl");
	shaders["image_shader"].set_culling(false);
	shaders["image_shader"].set_depth_testing(true);

	shaders["user_interface_shader"].id = create_shader_program("ui_vertex_shader.glsl", "ui_fragment_shader.glsl");
	shaders["user_interface_shader"].set_culling(false);
	shaders["user_interface_shader"].set_depth_testing(false);

	shaders["default_shader"].id = create_shader_program("vertex_shader.glsl", "fragment_shader.glsl");
	shaders["default_shader"].set_culling(true);
	shaders["default_shader"].set_depth_testing(true);

	shaders["physics_shader"].id = create_shader_program("physics_vertex_shader.glsl", "physics_fragment_shader.glsl");
	shaders["physics_shader"].set_culling(false);
	shaders["physics_shader"].set_depth_testing(true);

	shaders["skybox_shader"].id = create_shader_program("skybox_vertex_shader.glsl", "skybox_fragment_shader.glsl");
	shaders["skybox_shader"].set_depth_testing(true);

	shaders["shadow_shader"].id = create_shader_program("shadow_vertex_shader.glsl", "shadow_fragment_shader.glsl");
	shaders["shadow_shader"].set_culling(true);
	shaders["shadow_shader"].set_depth_testing(true);

	shaders["debug_shader"].id = create_shader_program("debug_vertex_shader.glsl", "debug_fragment_shader.glsl");
	shaders["debug_shader"].set_culling(false);
	shaders["debug_shader"].set_depth_testing(true);
}

void Shader::use_shader(std::string shader_name)
{
	Shader shader = shaders.find(shader_name)->second;

	if (shader.culling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(shader.culling_mode);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	if (shader.depth_test)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	glUseProgram(shader.get_id());
	current_shader = shader.get_id();
}

GLuint Shader::get_current_shader()
{
	return current_shader;
}

GLuint Shader::get_id()
{
	return id;
}

void Shader::set_depth_testing(bool b)
{
	depth_test = b;
}

void Shader::set_culling(bool b)
{
	culling = b;
}
void Shader::set_culling_mode(GLenum cull_mode)
{
	culling_mode = cull_mode;
}

GLuint Shader::get_shader(std::string shader_name)
{
	auto it = shaders.find(shader_name);
	if (it == shaders.end())
	{
		std::clog << "No shader with the name '" << shader_name << "' exists." << std::endl;
		return 0;
	}
	return it->second.get_id();
}

GLuint Shader::create_shader(std::string shader_type, std::string path)
{
	std::ifstream file;
	std::string shader_source = "";
	std::string line;

	GLuint shader;
	GLint result;
	GLchar info_log[512];
	const GLchar* source;

	file.open(path);

	while (std::getline(file, line))
	{
		shader_source.append(line + "\n");
	}

	source = shader_source.c_str();

	if (shader_type == "vertex")
	{
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (shader_type == "fragment")
	{
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(shader, 512, nullptr, info_log);
		std::clog << "SHADER COMPILATION FAILED\n" << info_log << std::endl;
		return 0;
	}
	file.close();
	return shader;
}

GLuint Shader::create_shader_program(std::string vs_path, std::string fs_path)
{
	std::string path =
		"C:/Users/sharg_000/Documents/ThirdPersonShooter_GL/ThirdPersonShooter_GL/shaders/";

	GLint result;
	GLchar info_log[512];
	GLuint vertex_shader = create_shader("vertex", path + vs_path);
	GLuint fragment_shader = create_shader("fragment", path + fs_path);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
		std::clog << "SHADER LINKING FAILED\n" << info_log << std::endl;
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
}