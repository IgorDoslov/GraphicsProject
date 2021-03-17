#include "gl_core_4_4.h"

#include "HeightMapStarterApp.h"
#include "Gizmos.h"
#include "Input.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <fstream>
#include <streambuf>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

HeightMapStarterApp::HeightMapStarterApp() {

}

HeightMapStarterApp::~HeightMapStarterApp() {

}

bool HeightMapStarterApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	m_heightmap = new aie::Texture("./HeightMapStarter/images/heightmap1.bmp");
	m_tileTexture = new aie::Texture("./HeightMapStarter/images/tile.png");

	m_heightmapMesh.MakePlane(m_heightmap);
	m_cubeMesh.MakeCube();
	
	LoadShader();

	return true;
}

void HeightMapStarterApp::shutdown() {

	UnloadShader();

	m_cubeMesh.Destroy();
	m_heightmapMesh.Destroy();

	delete m_tileTexture;
	delete m_heightmap;

	Gizmos::destroy();
}

void HeightMapStarterApp::update(float deltaTime) {

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	UpdateCamera(deltaTime);

	input->getMouseXY(&m_lastMouseXPos, &m_lastMouseYPos);
}

void HeightMapStarterApp::draw() {

	glm::mat4 identity;

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	// setup the shader
	glUseProgram(m_shader);

	// dend the model, view and projection matrices
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "projection"), 1, false, glm::value_ptr(m_projectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "view"), 1, false, glm::value_ptr(m_viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "model"), 1, false, glm::value_ptr(identity));

	// send the textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_tileTexture->getHandle());
	glUniform1i(glGetUniformLocation(m_shader, "texture"), 0);

	// Draw the heightmap
	m_heightmapMesh.Draw();

	m_cubeMesh.Draw();

	glUseProgram(0);

	DarwGizmosGrid();
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
	Gizmos::clear(); // clear gizmos for next frame.
}

void HeightMapStarterApp::UpdateCamera(float deltaTime)
{
	aie::Input *input = aie::Input::getInstance();
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT) || input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		const float mouseSensitivity = 10.0f * deltaTime;

		int mouseX, mouseY;
		float xOffset, yOffset;
		aie::Input *input = aie::Input::getInstance();
		input->getMouseXY(&mouseX, &mouseY);

		// get the change in the mouse movement.
		xOffset = (mouseX - m_lastMouseXPos) * mouseSensitivity;
		yOffset = (mouseY - m_lastMouseYPos) * mouseSensitivity;

		// ajdust yaw and pitch based on movement
		m_cameraYaw += xOffset;
		m_cameraPitch += yOffset;

		if (m_cameraPitch > 89.0f) m_cameraPitch = 89.0f;
		if (m_cameraPitch < -89.0f) m_cameraPitch = -89.0f;

		glm::vec3 front;
		front.x = glm::cos(glm::radians(m_cameraYaw)) * glm::cos(glm::radians(m_cameraPitch));
		front.y = glm::sin(glm::radians(m_cameraPitch));
		front.z = glm::sin(glm::radians(m_cameraYaw)) * glm::cos(glm::radians(m_cameraPitch));
		m_cameraFront = glm::normalize(front);
	}

	const float cameraSpeed = 5.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_cameraPos += cameraSpeed * m_cameraFront;

	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_cameraPos -= cameraSpeed * m_cameraFront;

	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;

	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;

	m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void HeightMapStarterApp::DarwGizmosGrid()
{
	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i) 
	{
		Gizmos::addLine(
			vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);

		Gizmos::addLine(
			vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));
}

std::string HeightMapStarterApp::StringFromFile(const char *filename)
{
	std::ifstream file(filename);
	std::string fileString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return fileString;
}
void HeightMapStarterApp::LoadShader()
{
	std::string vsSource = StringFromFile("./HeightMapStarter/shaders/DiffuseLight.vert");
	const char *vsShaderSrcPtr = vsSource.c_str();

	std::string fsSource = StringFromFile("./HeightMapStarter/shaders/DiffuseLight.frag");
	const char *fsShaderSrcPtr = fsSource.c_str();
	// Step 1:
	// Load the vertex shader, provide it with the source code and compile it.
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsShaderSrcPtr, NULL);
	glCompileShader(vs);

	// Step 2:
	// Load the fragment shader, provide it with the source code and compile it.
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsShaderSrcPtr, NULL);
	glCompileShader(fs);

	// Step 3:
	// Create the shader program
	m_shader = glCreateProgram();

	glAttachShader(m_shader, vs);
	glAttachShader(m_shader, fs);

	// Bind Attribute Locations
	glBindAttribLocation(m_shader, 0, "vPosition");
	glBindAttribLocation(m_shader, 1, "vColor");
	glBindAttribLocation(m_shader, 2, "vuv");

	glLinkProgram(m_shader);

	// Step 6:
	// destroy the vertex and fragment shader, we are finished with them
	// as they have been combined into the shaderProgram
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void HeightMapStarterApp::UnloadShader()
{
	glDeleteProgram(m_shader);
}

void Vertex::SetupVertexAttribPointers()
{
	// enable vertex position element
	// notice when we loaded the shader, we described the "position" element to be location 0.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0 (position)
		4,                  // size - how many floats make up the position (x, y, z, w)
		GL_FLOAT,           // type - our x,y,z, w are float values
		GL_FALSE,           // normalized? - not used
		sizeof(Vertex),     // stride - size of an entire vertex
		(void*)0            // offset - bytes from the beginning of the vertex
	);

	// enable vertex color element
	// notice when we loaded the shader, we described the "color" element to be location 1.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute 1 (color)
		4,                  // size - how many floats make up the color (r, g, b, a)
		GL_FLOAT,           // type - our r,g,b,a are float values
		GL_FALSE,           // normalized? - not used
		sizeof(Vertex),     // stride - size of an entire vertex
		(void*)(sizeof(float) * 4)            // offset - bytes from the beginning of the vertex
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                  // attribute 1 (uv)
		2,                  // size - how many floats make up the uv coords
		GL_FLOAT,           // type - our u,v are float values
		GL_FALSE,           // normalized? - not used
		sizeof(Vertex),     // stride - size of an entire vertex
		(void*)(sizeof(float) * 8)            // offset - bytes from the beginning of the vertex
	);
}

void GLMesh::MakeCube()
{
	glm::vec4 white(1, 1, 1, 1);

	// Step 1:
	// Specify the position and color for each vert of a cube
	// for this example, each face does not share a vert, so we have 4 verts for each face of our cube/
	Vertex verts[] = {
		// POSITION					COLOR	
		// FRONT FACE				  - RED
		{ {-0.5f,-0.5f, 0.5f, 1.0f },white,{ 0.0f, 0.0f } },	// 0
		{ { 0.5f,-0.5f, 0.5f, 1.0f }, white,{ 1.0f, 0.0f } },	// 1
		{ { 0.5f, 0.5f, 0.5f, 1.0f }, white,{ 1.0f, 1.0f } },	// 2
		{ {-0.5f, 0.5f, 0.5f, 1.0f },white,{ 0.0f, 1.0f } },	// 3

														 		// BACK FACE				- YELLOW
		{ {-0.5f,-0.5f,-0.5f, 1.0f },white,{ 0.0f, 0.0f } },	// 4
		{ { 0.5f,-0.5f,-0.5f, 1.0f },white,{ 1.0f, 0.0f } },	// 5
		{ { 0.5f, 0.5f,-0.5f, 1.0f },white,{ 1.0f, 1.0f } },	// 6
		{ {-0.5f, 0.5f,-0.5f, 1.0f },white,{ 0.0f, 1.0f } },	// 7

																// LEFT FACE				- GREEN
		{ {-0.5f,-0.5f,-0.5f, 1.0f },white,{ 0.0f, 0.0f } },	// 8
		{ {-0.5f,-0.5f, 0.5f, 1.0f },white,{ 0.0f, 1.0f } },	// 9
		{ {-0.5f, 0.5f, 0.5f, 1.0f },white,{ 1.0f, 1.0f } },	// 10
		{ {-0.5f, 0.5f,-0.5f, 1.0f },white,{ 1.0f, 0.0f } },	// 11

																// RIGHT FACE				- CYAN
		{ { 0.5f,-0.5f,-0.5f, 1.0f },white,{ 0.0f, 0.0f } },	// 12
		{ { 0.5f,-0.5f, 0.5f, 1.0f },white,{ 0.0f, 1.0f } },	// 13
		{ { 0.5f, 0.5f, 0.5f, 1.0f },white,{ 1.0f, 1.0f } },	// 14
		{ { 0.5f, 0.5f,-0.5f, 1.0f },white,{ 1.0f, 0.0f } },	// 15

																// TOP FACE					 - BLUE 
		{ {-0.5f, 0.5f,-0.5f, 1.0f },white,{ 0.0f, 0.0f } },	// 16
		{ {-0.5f, 0.5f, 0.5f, 1.0f },white,{ 0.0f, 1.0f } },	// 17
		{ { 0.5f, 0.5f, 0.5f, 1.0f },white,{ 1.0f, 1.0f } },	// 18
		{ { 0.5f, 0.5f,-0.5f, 1.0f },white,{ 1.0f, 0.0f } },	// 19

																// BOTTOM FACE				- MAJENTA
		{ {-0.5f,-0.5f,-0.5f, 1.0f },white,{ 0.0f, 0.0f } },	// 20
		{ {-0.5f,-0.5f, 0.5f, 1.0f },white,{ 0.0f, 1.0f } },	// 21
		{ { 0.5f,-0.5f, 0.5f, 1.0f },white,{ 1.0f, 1.0f } },	// 22
		{ { 0.5f,-0.5f,-0.5f, 1.0f },white,{ 1.0f, 0.0f } }		// 23
	};

	// Step 2:
	// From the above verts, we need to construct triangles that opengl can then use to render.
	// Winding order is important, each triangle needs to be described in a clockwise order
	// this defines the faceing direction for the triangle.
	// By default OpenGL will cull pixels that are "facing away" from the camera glCullMode(GL_BACK) is the default setting.
	// Culling can be changed to GL_FRONT or GL_FRONT_AND_BACK, or enabled/dsabled via glEnable(GL_CULL_FACE) / glDisable(GL_CULL_FACE)
	unsigned short indices[] =
	{
		0, 1, 2,	 0, 2, 3,  // front face
		6, 5, 4,	 7, 6, 4,  // back face
		8,9,10,	 8,10,11,  // left face
		14,13,12,	 15,14,12, // right face
		16,17,18,	 16,18,19, // top face
		22,21,20,	 23,22,20  // bottom face
	};

	// Step 3:
	// Its always a good idea to keep track of how many verts and indices we have
	// When drawing the glDrawElements method requires the number if indices
	// sizeof(verts) returns the entire size in bytes of the array
	// sizeof(Vertex) returns the size in bytes of a single vertes
	// we can calculate the number of verts or indices by dividing.
	m_numIndices = sizeof(indices) / sizeof(unsigned short);

	// Step 4:
	// Generate the VAO and Bind bind it.
	// Our VBO (vertex buffer object) and IBO (Index Buffer Object) will be "grouped" with this VAO
	// other settings will also be grouped with the VAO. this is used so we can reduce draw calls in the render method.
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Step 5:
	// Create our VBO and IBO.
	// Then tell Opengl what type of buffer they are used for
	// VBO a buffer in graphics memory to contains our vertices
	// IBO a buffer in graphics memory to contain our indices.
	// Then Fill the buffers with our generated data.
	// This is taking our verts and indices from ram, and sending them to the graphics card
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Step 6:
	// Vertices can have any shape, for us, each vertex has a position and color.
	// The "shape" of our vertex need to be described to OepnGL
	// This is so the vertices can be sent to our shader and be mapped to the correct variables locations.
	Vertex::SetupVertexAttribPointers();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void GLMesh::MakePlane(aie::Texture *heighmap)
{
	const unsigned char *pixels = heighmap->getPixels();

	int xDiv = heighmap->getWidth();
	int zDiv = heighmap->getHeight();

	std::vector<Vertex> verts;
	std::vector<unsigned short> indices;

	// build the height map
	for (int z = 0; z<zDiv; z++)
	{
		for (int x = 0; x<xDiv; x++)
		{
			unsigned int i = (z * xDiv + x);

			// get the value of the r component at the pixel located at x,z
			// the value returned is between 0 and 255 divided by 255 for a value between 0 and 1
			// minue 0.5 so we have a value between -.05 and +0.5
			// multiply by 2 for a value between -1 and 1
			float y = ((pixels[i * 3] / 255.0f) - 0.5f);

			// position of vertex
			float xPos = (x*0.1f) - (xDiv*0.1f*0.5f);
			float yPos = y * 2;
			float zPos = (z*0.1f) - (zDiv*0.1f*0.5f);

			Vertex vert{
				{ xPos, yPos, zPos, 1.0f },
				{ 1.0f, 1.0f, 1.0f, 1.0f },
				{ (float)x / (xDiv - 1), (float)z / (zDiv - 1) }
			};

			verts.push_back(vert);
		}
	}

	// calculate indices
	for (unsigned int z = 0; z<zDiv - 1; z++)
	{
		for (unsigned int x = 0; x<xDiv - 1; x++)
		{
			unsigned int i = z * xDiv + x;

			indices.push_back(i + 1);		// 1--0
			indices.push_back(i);			// | /
			indices.push_back(i + xDiv);	// 2

			indices.push_back(i + 1);			//    0
			indices.push_back(i + xDiv);		//  / |
			indices.push_back(i + xDiv + 1);	// 2--1
		}
	}

	m_numIndices = indices.size();

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	// populate the buffers with the verts passed in
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), &indices[0], GL_STATIC_DRAW);

	Vertex::SetupVertexAttribPointers();

	// un-bind the buffers;
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLMesh::Destroy()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteVertexArrays(1, &m_vao);
}

void GLMesh::Draw()
{
	// Step 2: Bind the VAO
	// When we setup the geometry, we did a bunch of glEnableVertexAttribArray and glVertexAttribPointer method calls
	// we also Bound the vertex array and index array via the glBindBuffer call.
	// if we where not using VAO's we would have to do thoes method calls each frame here.
	glBindVertexArray(m_vao);

	// Step 3: Draw Elements. We are using GL_TRIANGLES.
	// we need to tell openGL how many indices there are, and the size of our indices
	// when we setup the geometry, our indices where an unsigned char (1 byte for each indicy)
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_SHORT, 0);

	// Step 5: Now that we are done drawing the geometry
	// unbind the vao, we are basicly cleaning the opengl state
	glBindVertexArray(0);
}