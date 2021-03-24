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

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// === SHADER ===
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_normalMapShader;

	// ==============
	// Create a bunny with a flat colour
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;

	// Create a Soulspear
	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	// Create a sword
	aie::OBJMesh m_swordMesh;
	glm::mat4 m_swordTransform;

	Scene* m_scene;

	// Leg 1
	KeyFrame m_hipFrames[2];
	KeyFrame m_kneeFrames[2];
	KeyFrame m_ankleFrames[2];

	glm::mat4 m_hipBone;
	glm::mat4 m_kneeBone;
	glm::mat4 m_ankleBone;

	// Leg 2
	KeyFrame m_hipFrames2[2];
	KeyFrame m_kneeFrames2[2];
	KeyFrame m_ankleFrames2[2];

	glm::mat4 m_hipBone2;
	glm::mat4 m_kneeBone2;
	glm::mat4 m_ankleBone2;

	// Leg 3
	KeyFrame m_hipFrames3[2];
	KeyFrame m_kneeFrames3[2];
	KeyFrame m_ankleFrames3[2];

	glm::mat4 m_hipBone3;
	glm::mat4 m_kneeBone3;
	glm::mat4 m_ankleBone3;

	// Leg 4
	KeyFrame m_hipFrames4[2];
	KeyFrame m_kneeFrames4[2];
	KeyFrame m_ankleFrames4[2];

	glm::mat4 m_hipBone4;
	glm::mat4 m_kneeBone4;
	glm::mat4 m_ankleBone4;

	// Ambient light
	glm::vec3 m_ambLight = glm::vec3(0.25f);

	// Sunlight
	glm::vec3 sunlightMov = { 0.0f, 0.0f, 0.0f };
	float sunLightOrbit = 2.0f;
	float sunLightOrbitSpeed = 2.0f;


public:
	bool LoadShaderAndMeshLogic(Light a_light);

	void IMGUI_Logic();

	void LegPosRot(int frameNum, KeyFrame* hipFrame, KeyFrame* kneeFrame, KeyFrame* ankleFrame,
		glm::vec3 hipPos, glm::quat hipRot, glm::vec3 kneePos, glm::quat kneeRot, glm::vec3 anklePos, glm::quat ankleRot);

	void AnimateLeg(KeyFrame* hipFrames, KeyFrame* kneeFrames, KeyFrame* ankleFrames, glm::mat4& hipBone, glm::mat4& kneeBone, glm::mat4& ankleBone);

	void DrawLeg(glm::mat4 hipBone, glm::mat4 kneeBone, glm::mat4 ankleBone, glm::vec4 legColor);

};