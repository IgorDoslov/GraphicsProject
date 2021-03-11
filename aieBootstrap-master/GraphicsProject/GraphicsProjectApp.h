#pragma once

#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include "OBJMesh.h"

class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// === SHADER ===
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_bunnyShader;
	aie::ShaderProgram m_phongShader;
	// ==============
	// Basic plane
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;


	// Create a bunny with a flat colour
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;

	struct Light {
		glm::vec3 direction;
		glm::vec3 colour;
	};

	Light m_light;
	glm::vec3 m_ambientLight;



	glm::vec3 m_camPosition = glm::vec3(10);

public:
	bool LoadShaderAndMeshLogic();
	void DrawShaderAndMeshes(glm::mat4, glm::mat4);
};