#pragma once

#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include "OBJMesh.h"
#include "Camera.h"
#include <Vector>
#include "Scene.h"


class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	int m_currentCamera = 0;
	
	Camera m_camera;
	std::vector<Camera> m_cameras;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	// === SHADER ===
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_bunnyShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_normalMapShader;
	aie::ShaderProgram m_normalMapMultiLights;


	// ==============
	// Basic plane
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;


	// Create a bunny with a flat colour
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;
	glm::vec3 m_bunnyPosition = {0,0,0};

	// Create a dragon with a flat colour
	aie::OBJMesh m_dragonMesh;
	glm::mat4 m_dragonTransform;


	// Create a Buddha with a flat colour
	aie::OBJMesh m_buddhaMesh;
	glm::mat4 m_buddhaTransform;

	// Create a Lucy with a flat colour
	aie::OBJMesh m_lucyMesh;
	glm::mat4 m_lucyTransform;

	// Create a Soulspear
	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	// Create a sword
	aie::OBJMesh m_swordMesh;
	glm::mat4 m_swordTransform;

	Scene* m_scene;

	/*struct Light {

		glm::vec3 direction;
		glm::vec3 colour;
		
	};*/

	//Light m_light;
	//glm::vec3 m_ambientLight;
	

	//std::vector<Light> m_lights;
	glm::vec3 m_camPosition = glm::vec3(15);

public:
	bool LoadShaderAndMeshLogic(Light a_light);
	// void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	void IMGUI_Logic();
};