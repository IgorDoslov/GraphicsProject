#pragma once

#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include "OBJMesh.h"
#include "Camera.h"
#include <Vector>
#include "Scene.h"

#define GLM_ENABLE_EXPERIMENTAL #include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

struct KeyFrame {
	glm::vec3 position;
	glm::quat rotation;
};




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
	std::vector<Camera*> m_cameras;

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
	glm::vec3 m_bunnyPosition = { 0,0,0 };

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

	KeyFrame m_hipFrames[2];
	KeyFrame m_kneeFrames[2];
	KeyFrame m_ankleFrames[2];

	glm::mat4 m_hipBone;
	glm::mat4 m_kneeBone;
	glm::mat4 m_ankleBone;

	KeyFrame m_hipFrames2[2];
	KeyFrame m_kneeFrames2[2];
	KeyFrame m_ankleFrames2[2];

	glm::mat4 m_hipBone2;
	glm::mat4 m_kneeBone2;
	glm::mat4 m_ankleBone2;

	/*glm::vec3 m_positions[2];
	glm::quat m_rotations[2];*/
	/*struct Light {

		glm::vec3 direction;
		glm::vec3 colour;

	};*/

	//Light m_light;
	glm::vec3 m_ambLight = glm::vec3(0.25f);
	
	glm::vec3 sunlightMov = { 0.0f, 0.0f, 0.0f };
	float sunLightOrbit = 0.0f;
	float sunLightOrbitSpeed = 1.0f;



	//std::vector<Light> m_lights;
	glm::vec3 m_camPosition = glm::vec3(15);

public:
	bool LoadShaderAndMeshLogic(Light a_light);
	// void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	void IMGUI_Logic();

	void LegPosRot(int frameNum, KeyFrame* hipFrame, KeyFrame* kneeFrame, KeyFrame* ankleFrame, 
		glm::vec3 hipPos, glm::quat hipRot, glm::vec3 kneePos, glm::quat kneeRot, glm::vec3 anklePos, glm::quat ankleRot);
	
	
	
};