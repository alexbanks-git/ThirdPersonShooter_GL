#include "graphics.hpp"

static SDL_Window* window;
static GLint window_width;
static GLint window_height;
static GLuint then;
static GLuint now;
static GLfloat target_fps;
static GLfloat frame_delay;
static GLuint default_shader;
static GLuint user_interface_shader;
static GLuint debug_shader;
static GLuint skybox_shader;
static GLfloat elapsed_time;
static GLuint image_shader;

GLuint Graphics::get_default_shader()
{
	return default_shader;
}

GLuint Graphics::get_image_shader()
{
	return image_shader;
}

GLuint Graphics::get_user_interface_shader()
{
	return user_interface_shader;
}

GLuint Graphics::get_debug_shader()
{
	return debug_shader;
}

GLuint Graphics::get_skybox_shader()
{
	return skybox_shader;
}

GLint Graphics::init_graphics(GLint w, GLint h, GLfloat fps)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::clog << "Error initializing SDL" << std::endl;
	}

	window_width = w;
	window_height = h;
	target_fps = fps;
	frame_delay = 1000.0f/target_fps;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	window = SDL_CreateWindow(
		"TPS Game",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		std::clog << "Failed to create window!\n" << std::endl;
		return 0;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);

	if (context == nullptr)
	{
		std::clog << "Failed to create context!\n" << std::endl;
		return 0;
	}

	SDL_GL_MakeCurrent(window, context);

	GLint glew_status = glewInit();
	if (glew_status != 0)
	{
		std::clog << "Error: " << glewGetErrorString(glew_status) << std::endl;
		return 0;
	}
	image_shader = create_shader_program("image_vertex_shader.glsl", "image_fragment_shader.glsl");
	user_interface_shader = create_shader_program("ui_vertex_shader.glsl", "ui_fragment_shader.glsl");
	default_shader = create_shader_program("vertex_shader.glsl", "fragment_shader.glsl");
	debug_shader = create_shader_program("physics_vertex_shader.glsl", "physics_fragment_shader.glsl");
	skybox_shader = create_shader_program("skybox_vertex_shader.glsl", "skybox_fragment_shader.glsl");
	return 1;
}

SDL_Window* Graphics::get_window()
{
	return window;
}

int Graphics::get_window_width()
{
	return window_width;
}

int Graphics::get_window_height()
{
	return window_height;
}

void Graphics::update_window()
{
	then = now;
	SDL_GL_SwapWindow(window);
	now = SDL_GetTicks();
	if ((now - then) < frame_delay)
	{
		SDL_Delay(frame_delay - (now - then));
	}
	elapsed_time = (SDL_GetTicks() - then) / 1000.0f;
}

GLfloat Graphics::get_elapsed_time()
{
	return elapsed_time;
}

GLuint Graphics::create_shader(std::string shader_type, std::string path)
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

GLuint Graphics::create_shader_program(std::string vs_path, std::string fs_path)
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

void Graphics::draw_mesh(Mesh* mesh)
{
	if (mesh == nullptr)
	{
		std::clog << "Graphics::draw_mesh: mesh is null" << std::endl;
		return;
	}
	mesh->bind();

	glDrawElements(GL_TRIANGLES, mesh->get_index_count(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}