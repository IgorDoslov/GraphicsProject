#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>
#include <vector>
#include "Scene.h"
#include "Instance.h"


#define GLM_ENABLE_EXPERIMENTAL #include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;


GraphicsProjectApp::GraphicsProjectApp() //: m_ambientLight(), m_bunnyTransform(), m_buddhaTransform(), m_dragonTransform(), m_lucyTransform(), m_light(), m_projectionMatrix(), m_quadTransform(), m_viewMatrix()
{



}

GraphicsProjectApp::~GraphicsProjectApp() {

}

bool GraphicsProjectApp::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);


	Light light;
	light.m_colour = { 1, 1, 1 };
	light.m_direction = { 1, -1, 1 };

	// Leg 1 Red
	// Start
	LegPosRot(0, m_hipFrames, m_kneeFrames, m_ankleFrames,
		glm::vec3(0, 5, 0), glm::vec3(1, 0, 0), // hip
		glm::vec3(0, -2.5f, 0), glm::vec3(1, 0, 0), // knee
		glm::vec3(0, -2.5f, 0), glm::vec3(-1, 0, 0)); // ankle
	// End
	LegPosRot(1, m_hipFrames, m_kneeFrames, m_ankleFrames,
		glm::vec3(0, 5, 0), glm::vec3(-1, 0, 0), // hip
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0), // knee
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0)); // ankle

	// Leg 2 Green
	// Start
	LegPosRot(0, m_hipFrames2, m_kneeFrames2, m_ankleFrames2,
		glm::vec3(5, 5, 0), glm::vec3(-1, 0, 0), // hip
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0), // knee
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0)); // ankle
	// End
	LegPosRot(1, m_hipFrames2, m_kneeFrames2, m_ankleFrames2,
		glm::vec3(5, 5, 0), glm::vec3(1, 0, 0), // hip
		glm::vec3(0, -2.5f, 0), glm::vec3(1, 0, 0), // knee
		glm::vec3(0, -2.5f, 0), glm::vec3(-1, 0, 0)); // ankle

	// Leg 3 Blue
	// Start
	LegPosRot(0, m_hipFrames3, m_kneeFrames3, m_ankleFrames3,
		glm::vec3(5, 5, 5), glm::vec3(1, 0, 0), // hip
		glm::vec3(0, -2.5f, 0), glm::vec3(1, 0, 0), // knee
		glm::vec3(0, -2.5f, 0), glm::vec3(-1, 0, 0)); // ankle
	// End
	LegPosRot(1, m_hipFrames3, m_kneeFrames3, m_ankleFrames3,
		glm::vec3(5, 5, 5), glm::vec3(-1, 0, 0), // hip
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0), // knee
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0)); // ankle

	// Leg 4 White
	// Start
	LegPosRot(0, m_hipFrames4, m_kneeFrames4, m_ankleFrames4,
		glm::vec3(0, 5, 5), glm::vec3(-1, 0, 0), // hip
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0), // knee
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0)); // ankle
	// End
	LegPosRot(1, m_hipFrames4, m_kneeFrames4, m_ankleFrames4,
		glm::vec3(0, 5, 5), glm::vec3(1, 0, 0), // hip
		glm::vec3(0, -2.5f, 0), glm::vec3(1, 0, 0), // knee
		glm::vec3(0, -2.5f, 0), glm::vec3(-1, 0, 0)); // ankle

	return LoadShaderAndMeshLogic(light);
}

void GraphicsProjectApp::shutdown() {

	Gizmos::destroy();
	delete m_scene;
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
	// Sunlight
	Gizmos::addSphere(-sunLightOrbit * m_scene->GetLight().m_direction + sunlightMov, 0.25f, 8, 8, glm::vec4(m_scene->GetLight().m_colour, 1));
	// Light red
	Gizmos::addSphere(m_scene->GetPointLightPositions()[0], 0.25f, 8, 8, glm::vec4(m_scene->GetPointLights()[0].m_colour, 1));
	// Light 2
	Gizmos::addSphere(m_scene->GetPointLights()[1].m_direction, 0.25f, 8, 8, glm::vec4(m_scene->GetPointLights()[1].m_colour, 1));


	m_scene->GetLight().m_direction = glm::normalize(glm::vec3(glm::cos(time * sunLightOrbitSpeed), (glm::sin(time * sunLightOrbitSpeed)), 0));


	m_scene->SetAmbientLight(m_ambLight);

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// Only one camera can move
	if (m_scene->GetCurrentCam()->m_isStatic == false)
		m_scene->GetCurrentCam()->Update(deltaTime);


	//m_scene->Update(deltaTime);
	IMGUI_Logic();

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();



	AnimateLeg(m_hipFrames, m_kneeFrames, m_ankleFrames, m_hipBone, m_kneeBone, m_ankleBone);
	AnimateLeg(m_hipFrames2, m_kneeFrames2, m_ankleFrames2, m_hipBone2, m_kneeBone2, m_ankleBone2);
	AnimateLeg(m_hipFrames3, m_kneeFrames3, m_ankleFrames3, m_hipBone3, m_kneeBone3, m_ankleBone3);
	AnimateLeg(m_hipFrames4, m_kneeFrames4, m_ankleFrames4, m_hipBone4, m_kneeBone4, m_ankleBone4);


	



	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	glm::mat4 projectionMatrix = m_scene->GetCurrentCam()->GetProjectionMatrix((float)getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_scene->GetCurrentCam()->GetViewMatrix();


	m_scene->Draw();


	DrawLeg(m_hipBone, m_kneeBone, m_ankleBone, glm::vec4(1,0,0,1));
	DrawLeg(m_hipBone2, m_kneeBone2, m_ankleBone2, glm::vec4(0,1,0,1));
	DrawLeg(m_hipBone3, m_kneeBone3, m_ankleBone3, glm::vec4(0,0,1,1));
	DrawLeg(m_hipBone4, m_kneeBone4, m_ankleBone4, glm::vec4(1,1,1,1));



	Gizmos::draw(projectionMatrix * viewMatrix);
}

bool GraphicsProjectApp::LoadShaderAndMeshLogic(Light a_light)
{

#pragma region Phong Load
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	if (m_phongShader.link() == false)
	{
		printf("Phong Shader had an error: %s\n", m_phongShader.getLastError());
		return false;
	}

#pragma endregion



#pragma region NormalMapShader
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");

	if (m_normalMapShader.link() == false)
	{
		printf("Normal Map Shader had an error: %s\n", m_normalMapShader.getLastError());
		return false;
	}
#pragma endregion



#pragma region Soulspear
	// Load spear obj file
	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh has had an error!\n");
		return false;
	}


#pragma endregion

#pragma region Sword
	// Load sword obj file
	if (m_swordMesh.load("./sword/vikingsword.obj", true, true) == false)
	{
		printf("sword Mesh has had an error!\n");
		return false;
	}


#pragma endregion



#pragma region FlatBunny Logic
	// Logic
	if (m_bunnyMesh.load("./stanford/bunny.obj", true, true) == false)
	{
		printf("Bunny mesh Failed!\n");
		return false;
	}

#pragma endregion


	// Creating the four cameras for the scene
	m_cameras.push_back(new Camera(false, glm::vec3(1, 1, 1)));
	m_cameras.push_back(new Camera(true, glm::vec3(10, 0, 0)));
	m_cameras.push_back(new Camera(true, glm::vec3(0, 10, 0)));
	m_cameras.push_back(new Camera(true, glm::vec3(0, 0, 10)));


	m_scene = new Scene(m_cameras, glm::vec2(getWindowWidth(), getWindowHeight()), a_light, glm::vec3(0.25f));

	// Spear
	m_scene->AddInstance(new Instance(glm::vec3(2, 0, 0),
		glm::vec3(0, 3, 0),
		glm::vec3(1),
		&m_spearMesh,
		&m_normalMapShader));

	// Sword
	m_scene->AddInstance(new Instance(glm::vec3(0, 0, 5),
		glm::vec3(0, 0, 0),
		glm::vec3(0.01f),
		&m_swordMesh,
		&m_normalMapShader));

	// Bunny
	m_scene->AddInstance(new Instance(glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0.2f),
		&m_bunnyMesh,
		&m_phongShader));


	// Add a red light on the left side
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
	// Add a green light on the right side
	m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));


	return true;
}

void GraphicsProjectApp::IMGUI_Logic()
{
	// Change light direction and colour in the scene
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -1.f, 1.f);
	ImGui::DragFloat3("Sunlight Colour", &m_scene->GetLight().m_colour[0], 0.1f, 0.f, 2.0f);
	ImGui::DragFloat3("Ambient Colour", &m_ambLight[0], 0.1f, 0.f, 3.0f);

	ImGui::DragFloat3("Sunlight position", &sunlightMov[0], 0.1f, -50.f, 50.0f);
	ImGui::DragFloat("Sunlight orbit", &sunLightOrbit, 0.1f, -50.f, 50.0f);
	ImGui::DragFloat("Sunlight orbit speed", &sunLightOrbitSpeed, 0.1f, -50.f, 50.0f);

	ImGui::DragFloat3("light 1 position", &m_scene->GetPointLights()[0].m_direction[0], 0.1f, -50.f, 50.0f);
	ImGui::DragFloat3("light 1 colour", &m_scene->GetPointLights()[0].m_colour[0], 0.1f, 0.0f, 2.0f);
	ImGui::DragFloat3("light 2 position", &m_scene->GetPointLights()[1].m_direction[0], 0.1f, -50.f, 50.0f);
	ImGui::DragFloat3("light 2 colour", &m_scene->GetPointLights()[1].m_colour[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();

	// These allow you to move the models around in the scene
	ImGui::Begin("Object Positions");

	ImGui::DragFloat3("Bunny position", &m_scene->m_instances[2]->m_pos[0], 0.1f, -20.f, 20.0f);
	ImGui::DragFloat3("Bunny rotation", &m_scene->m_instances[2]->m_rot[0], 0.1f, -180.f, 180.0f);
	ImGui::DragFloat3("Bunny scale", &m_scene->m_instances[2]->m_scale[0], 0.1f, -20.f, 20.0f);

	ImGui::DragFloat3("Spear position", &m_scene->m_instances[0]->m_pos[0], 0.1f, -20.f, 20.0f);
	ImGui::DragFloat3("Spear rotation", &m_scene->m_instances[0]->m_rot[0], 0.1f, -180.f, 180.0f);
	ImGui::DragFloat3("Spear scale", &m_scene->m_instances[0]->m_scale[0], 0.1f, -20.f, 20.0f);

	ImGui::DragFloat3("Sword position", &m_scene->m_instances[1]->m_pos[0], 0.1f, -20.f, 20.0f);
	ImGui::DragFloat3("Sword rotation", &m_scene->m_instances[1]->m_rot[0], 0.1f, -180.f, 180.0f);
	ImGui::DragFloat3("Sword scale", &m_scene->m_instances[1]->m_scale[0], 0.1f, 0.1f, 2.0f);

	for (auto ins : m_scene->m_instances)
		ins->RecalculateTransform();

	ImGui::End();
	// Cameras
	ImGui::Begin("Camera Positions");
	ImGui::Text("Camera list");
	ImGui::TextColored(ImVec4(0, 1, 0, 1), ("Current camera: " + std::to_string(m_scene->currentCam)).c_str());
	for (int i = 0; i < m_scene->m_cameras.size(); i++)
	{
		if (ImGui::Button(("Select Camera" + std::to_string(i)).c_str(), ImVec2(100, 20)))
		{
			m_scene->currentCam = i;
		}
	}
	ImGui::End();


}


void GraphicsProjectApp::LegPosRot(int frameNum, KeyFrame* hipFrame, KeyFrame* kneeFrame, KeyFrame* ankleFrame,
	glm::vec3 hipPos, glm::quat hipRot, glm::vec3 kneePos, glm::quat kneeRot, glm::vec3 anklePos, glm::quat ankleRot)
{
	hipFrame[frameNum].position = hipPos;
	hipFrame[frameNum].rotation = hipRot;

	kneeFrame[frameNum].position = kneePos;
	kneeFrame[frameNum].rotation = kneeRot;

	ankleFrame[frameNum].position = anklePos;
	ankleFrame[frameNum].rotation = ankleRot;

}

void GraphicsProjectApp::AnimateLeg(KeyFrame* hipFrames, KeyFrame* kneeFrames, KeyFrame* ankleFrames, 
	glm::mat4 &hipBone, glm::mat4 &kneeBone, glm::mat4 &ankleBone)
{
	// animate leg
	float sHip = glm::cos(getTime()) * 0.5f + 0.5f;
	// linearly interpolate hip position
	glm::vec3 pHip = (1.0f - sHip) * hipFrames[0].position +
		sHip * hipFrames[1].position;
	// spherically interpolate hip rotation
	glm::quat rHip = glm::slerp(hipFrames[0].rotation,
		hipFrames[1].rotation, sHip);
	// update the hip bone
	hipBone = glm::translate(pHip) * glm::toMat4(rHip);

	// animate leg
	float sKnee = glm::cos(getTime()) * 0.5f + 0.5f;
	// linearly interpolate knee position
	glm::vec3 pKnee = (1.0f - sKnee) * kneeFrames[0].position +
		sKnee * kneeFrames[1].position;
	// spherically interpolate knee rotation
	glm::quat rKnee = glm::slerp(kneeFrames[0].rotation,
		kneeFrames[1].rotation, sKnee);
	// update the knee bone
	kneeBone = hipBone * glm::translate(pKnee) * glm::toMat4(rKnee);

	// animate leg
	float sAnkle = glm::cos(getTime()) * 0.5f + 0.5f;
	// linearly interpolate ankle position
	glm::vec3 pAnkle = (1.0f - sAnkle) * ankleFrames[0].position +
		sAnkle * ankleFrames[1].position;
	// spherically interpolate ankle rotation
	glm::quat rAnkle = glm::slerp(ankleFrames[0].rotation,
		ankleFrames[1].rotation, sAnkle);
	// update the ankle bone
	ankleBone = kneeBone * glm::translate(pAnkle) * glm::toMat4(rAnkle);
}

void GraphicsProjectApp::DrawLeg(glm::mat4 hipBone, glm::mat4 kneeBone, glm::mat4 ankleBone, glm::vec4 legColor)
{
	glm::vec3 hipPos = glm::vec3(hipBone[3].x,
		hipBone[3].y,
		hipBone[3].z);

	glm::vec3 kneePos = glm::vec3(kneeBone[3].x,
		kneeBone[3].y,
		kneeBone[3].z);

	glm::vec3 anklePos = glm::vec3(ankleBone[3].x,
		ankleBone[3].y,
		ankleBone[3].z);

	glm::vec4 half(0.5f);
	glm::vec4 pink(1, 0, 1, 1);

	Gizmos::addAABBFilled(hipPos, half, legColor, &hipBone);
	Gizmos::addAABBFilled(kneePos, half, legColor, &kneeBone);
	Gizmos::addAABBFilled(anklePos, half, legColor, &ankleBone);
}