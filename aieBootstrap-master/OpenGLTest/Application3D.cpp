#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "gl_core_4_4.h"

#include <string>
#include <fstream>
#include <streambuf>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	LoadShader();

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	
	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}

void Application3D::LoadShader()
{

	
	std::ifstream vsfile("./shaders/simple.vert");
	std::string vssrc = std::string(std::istreambuf_iterator<char>(vsfile), std::istreambuf_iterator<char>());
	
	std::ifstream fsfile("./shaders/simple.frag");
	std::string fssrc = std::string(std::istreambuf_iterator<char>(fsfile), std::istreambuf_iterator<char>());

	const char* vssrcPtr = vssrc.c_str();
	const char* fssrcPtr = fssrc.c_str();
	
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vssrcPtr, NULL);
	glCompileShader(vs);


	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fssrcPtr, NULL);
	glCompileShader(fs);

	m_shader = glCreateProgram();
	glAttachShader(m_shader, vs);
	glAttachShader(m_shader, fs);

	glLinkProgram(m_shader);

	int success = GL_TRUE;
	glGetProgramiv(m_shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_shader, GL_INFO_LOG_LENGTH, &infoLogLength);


		char *lastError = new char[infoLogLength + 1];
		glGetProgramInfoLog(m_shader, infoLogLength, 0, lastError);
		printf("%s\n", lastError);
		delete[] lastError;
	}



	glDeleteShader(vs);
	glDeleteShader(fs);
}