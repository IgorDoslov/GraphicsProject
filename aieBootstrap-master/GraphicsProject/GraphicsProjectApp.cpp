#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>


using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp() {

}

GraphicsProjectApp::~GraphicsProjectApp() {

}

bool GraphicsProjectApp::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	//m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_viewMatrix = glm::lookAt(m_camPosition, vec3(0), vec3(0, 1, 0));

	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	return LoadShaderAndMeshLogic();
}

void GraphicsProjectApp::shutdown() {

	Gizmos::destroy();
}

void GraphicsProjectApp::update(float deltaTime) {

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
	float time = getTime();
	//Gizmos::addSphere(vec3(0, 0, 0), 1, 16, 16, vec4(1, 1, 0, 1));
	Gizmos::addSphere(-vec3(glm::cos(time * 5), 0, glm::sin(time * 5)), 0.1, 8, 8, vec4(1, 0, 0, 1));
	Gizmos::addSphere(-vec3(glm::cos(time * 2), 0, glm::sin(time * 2)) + vec3(6), 0.1, 8, 8, vec4(1, 0, 0, 1));
	Gizmos::addSphere(-vec3(0, 0, 0), 1, 16, 16, vec4(1, 1, 0, 1));

	static float angle = 0;
	angle += deltaTime;
	m_camPosition.x = 10.f * sin(angle);
	m_camPosition.z = 10.f * cos(angle);
	m_viewMatrix = glm::lookAt(m_camPosition, vec3(0), vec3(0, 1, 0));



	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	DrawShaderAndMeshes(m_projectionMatrix, m_viewMatrix);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

bool GraphicsProjectApp::LoadShaderAndMeshLogic()
{
#pragma region Quad

	// Load the vertex shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	if (!m_simpleShader.link())
	{
		printf("Simple Shader had an error: %s\n", m_simpleShader.getLastError());
		return false;
	}

	//m_quadMesh.InitialiseQuad();
	// Define the six vertices for the two triangles that make the quad
	/*Mesh::Vertex verticesNoIndex[6];
	verticesNoIndex[0].position = { -0.5f, 0.f, 0.5f, 1.f };
	verticesNoIndex[1].position = { 0.5f, 0.f, 0.5f, 1.f };
	verticesNoIndex[2].position = { -0.5f, 0.f, -0.5f, 1.f };

	verticesNoIndex[3].position = { -0.5f, 0.f, -0.5f, 1.f };
	verticesNoIndex[4].position = { 0.5f, 0.f, 0.5f, 1.f };
	verticesNoIndex[5].position = { 0.5f, 0.f, -0.5f, 1.f };*/

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.f, 0.5f, 1.f };
	vertices[1].position = { 0.5f, 0.f, 0.5f, 1.f };
	vertices[2].position = { -0.5f, 0.f, -0.5f, 1.f };
	vertices[3].position = { 0.5f, 0.f, -0.5f, 1.f };

	unsigned int indices[6] = { 0,1,2,2,1,3 };

	m_quadMesh.Initialise(4, vertices, 6, indices);
	// We will make the quad 10 units by 10 units
	m_quadTransform = {
	10, 0, 0, 0,
	0, 10, 0, 0,
	0, 0, 10, 0,
	0, 0, 0, 1
	};
	float shadowLength = glm::sqrt(m_camPosition.x * m_camPosition.x + m_camPosition.z * m_camPosition.z);
	float angle = atan(shadowLength / m_camPosition.y);
	//m_quadTransform = glm::rotate(m_quadTransform, atan(1.41f), glm::vec3(10,0,-10));
	m_quadTransform = glm::rotate(m_quadTransform, angle, glm::vec3(10, 0, -10));
#pragma endregion

#pragma region FlatBunny
	// Load the vertex shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	if (!m_bunnyShader.link())
	{
		printf("Bunny Shader had an error: %s\n", m_bunnyShader.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny mesh Failed!\n");
		return false;
	}



	m_bunnyTransform = {
	0.5f, 0,0,0,
	0,0.5f,0,0,
	0,0,0.5f,0,
	0,0,0,1 };
#pragma endregion

	return true;
}

void GraphicsProjectApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
{
	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);

#pragma region Quad

	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform of the mesh
	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform; // PVM = Projection View Matrix
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	m_quadMesh.Draw();
#pragma endregion

#pragma region FlatBunny
	m_bunnyShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
	m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
	m_bunnyShader.bindUniform("MeshFlatColour", glm::vec4(0, 1, 0, 1));

	// Draw bunny mesh
	m_bunnyMesh.draw();
#pragma endregion

}
