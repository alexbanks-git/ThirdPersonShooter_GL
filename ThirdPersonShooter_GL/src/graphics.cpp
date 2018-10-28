#include "graphics.hpp"

static SDL_Window* window;
static SDL_Renderer* renderer;
static GLint window_width;
static GLint window_height;
static GLuint then;
static GLuint now;
static GLfloat target_fps;
static GLfloat frame_delay;
static glm::mat4 proj;
static glm::mat4 view;

static GLfloat elapsed_time;
static FT_Library ft_library;
static FT_Face ft_face;

void Graphics::set_matrices(glm::mat4 p, glm::mat4 v)
{
	proj = p;
	view = v;
}

glm::vec4 Graphics::to_world_coords(glm::vec4 vec, glm::mat4 model)
{
	//Add code
	return vec;
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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	if (renderer == nullptr)
	{
		std::clog << "Failed to create renderer context" << std::endl;
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
	Shader::init();

	//FT_Init_FreeType(&ft_library);
	//FT_New_Face(ft_library, "coolvetica rg.ttf", 0, &ft_face);
	//FT_Set_Pixel_Sizes(ft_face, 0, 48);

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

SDL_Renderer* Graphics::get_renderer()
{
	return renderer;
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

void Graphics::clear_screen(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}