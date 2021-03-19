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

	// create simple camera transforms
	//m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	//m_viewMatrix = glm::lookAt(m_camPosition, vec3(0), vec3(0, 1, 0));

	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	//m_light.colour = { 1, 1, 1 };
	//m_ambientLight = { 0.5f, 0.5f, 0.5f };

	//m_lights.push_back(Light{ glm::vec3{ 1, 0, 0 }, 3.0f * glm::vec3{1,1,1} });
	//m_lights.push_back(Light{ glm::vec3{ -1, 0, 0 }, 3.0f * glm::vec3{1,0,0} });
	//m_lights.push_back(Light{ glm::vec3{ 0, -1, 0 }, 3.0f * glm::vec3{0,1,0} });

	Light light;
	light.m_colour = { 1, 1, 1 };
	light.m_direction = { 1, -1, 1 };

	/*m_positions[0] = glm::vec3(10, 5, 10);
	m_positions[1] = glm::vec3(-10, 0, -10);
	m_rotations[0] = glm::quat(glm::vec3(0, -1, 0));
	m_rotations[1] = glm::quat(glm::vec3(0, 1, 0));*/


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

	//Gizmos::addSphere(vec3(0, 0, 0), 1, 16, 16, vec4(1, 1, 0, 1));
	//Gizmos::addSphere(-vec3(glm::cos(time * 5.f), 0.f, glm::sin(time * 5.0f)), 0.1f, 8.f, 8.f, vec4(1.f, 0.f, 0.f, 1.f));
	//Gizmos::addSphere(vec3(glm::cos(time * 2.f), 0.f, glm::sin(time * 2.f)) + planetMov, 0.1f, 8.f, 8.f, vec4(1.f, 0.f, 0.f, 1.f));
	//Gizmos::addSphere(-vec3(0.f, 0.f, 0.f), 1.f, 16.f, 16.f, vec4(1.f, 1.f, 0.f, 1.f));

	/*static float angle = 0;
	angle += deltaTime;
	m_camPosition.x = 10.f * sin(angle);
	m_camPosition.z = 10.f * cos(angle);*/
	//m_viewMatrix = glm::lookAt(m_camPosition, vec3(0), vec3(0, 1, 0));

	//m_bunnyTransform = glm::translate(m_bunnyTransform, m_bunnyPosition);


	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// Only one camera can move
	if (m_scene->currentCam == 0)
		m_scene->GetCurrentCam()->Update(deltaTime);

	//m_scene->Update(deltaTime);
	IMGUI_Logic();




	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	//// use time to animate a alue between [0, 1]
	//float s = glm::cos(getTime()) * 0.5f + 0.5f;
	//// standard linear interpolation
	//glm::vec3 p = (1.0f - s) * m_positions[0] + s * m_positions[1];
	//// quaternion slerp
	//glm::quat r = glm::slerp(m_rotations[0], m_rotations[1], s);
	//// build a matrix
	//glm::mat4 m = glm::translate(p) * glm::toMat4(r);
	//// draw a transform and box
	//Gizmos::addTransform(m);
	//Gizmos::addAABBFilled(p, glm::vec3(.5f), glm::vec4(1, 0, 0, 1), &m);


	m_hipFrames[0].position = glm::vec3(0, 5, 0);
	m_hipFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	m_hipFrames[1].position = glm::vec3(0, 5, 0);
	m_hipFrames[1].rotation = glm::quat(glm::vec3(-1, 0, 0));

	m_kneeFrames[0].position = glm::vec3(0, -2.5f, 0);
	m_kneeFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	m_kneeFrames[1].position = glm::vec3(0, -2.5f, 0);
	m_kneeFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));

	m_ankleFrames[0].position = glm::vec3(0, -2.5f, 0);
	m_ankleFrames[0].rotation = glm::quat(glm::vec3(-1, 0, 0));
	m_ankleFrames[1].position = glm::vec3(0, -2.5f, 0);
	m_ankleFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));

	// animate leg
	float s = glm::cos(getTime()) * 0.5f + 0.5f;
	// linearly interpolate hip position
	glm::vec3 p = (1.0f - s) * m_hipFrames[0].position +
		s * m_hipFrames[1].position;
	// spherically interpolate hip rotation
	glm::quat r = glm::slerp(m_hipFrames[0].rotation,
		m_hipFrames[1].rotation, s);
	// update the hip bone
	m_hipBone = glm::translate(p) * glm::toMat4(r);

	glm::vec3 hipPos = glm::vec3(m_hipBone[3].x,
		m_hipBone[3].y,
		m_hipBone[3].z);

	glm::vec3 kneePos = glm::vec3(m_kneeBone[3].x,
		m_kneeBone[3].y,
		m_kneeBone[3].z);

	glm::vec3 anklePos = glm::vec3(m_ankleBone[3].x,
		m_ankleBone[3].y,
		m_ankleBone[3].z);

	glm::vec4 half(0.5f);
	glm::vec4 pink(1, 0, 1, 1);

	Gizmos::addAABBFilled(hipPos, half, pink, &m_hipBone);
	Gizmos::addAABBFilled(kneePos, half, pink, &m_kneeBone);
	Gizmos::addAABBFilled(anklePos, half, pink, &m_ankleBone);

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	glm::mat4 projectionMatrix = m_scene->GetCurrentCam()->GetProjectionMatrix((float)getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_scene->GetCurrentCam()->GetViewMatrix();


	m_scene->Draw();

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
	m_cameras.push_back(new Camera(glm::vec3(1, 1, 1)));
	m_cameras.push_back(new Camera(glm::vec3(0, 1, 0)));
	m_cameras.push_back(new Camera(glm::vec3(3, 0, 0)));
	m_cameras.push_back(new Camera(glm::vec3(4, 0, 0)));


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

//void GraphicsProjectApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
//{
//	// Calculate the projection view matrix
//	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);
//
//#pragma region Quad
//
//	// Bind the shader
//	//m_textureShader.bind();
//
//	// Bind the transform of the mesh
//	//pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform; // PVM = Projection View Matrix
//	//m_textureShader.bindUniform("ProjectionViewModel", pvm);
//
//	// Bind the texture to a location of your choice (0)
//	//m_textureShader.bindUniform("diffuseTexture", 0);
//
//	// Bind the texture to the specified location
//	//m_gridTexture.bind(0);
//
//	// Draw the quad...
//	//m_quadMesh.Draw();
//#pragma endregion
//
//#pragma region FlatBunny
//	//m_bunnyShader.bind();
//	//pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
//	//m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
//	//m_bunnyShader.bindUniform("MeshFlatColour", glm::vec4(0, 1, 0, 1));
//
//	// Draw bunny mesh
//	//m_bunnyMesh.draw();
//
//
//#pragma endregion
//
//#pragma region Phong Shader
//
//	// Bind the shader
//	//m_phongShader.bind();
//
//	// Bind the camera position
//	//m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(a_viewMatrix)[3]));
//
//	//m_phongShader.bindUniform("AmbientColour", m_ambientLight);
//	//m_phongShader.bindUniform("LightColour", m_light.colour);
//	//m_phongShader.bindUniform("LightDirection", m_light.direction);
//
//	// Bind the light
//	//m_phongShader.bindUniform("LightDirection", m_light.direction);
//
//#pragma region Bunny
//
//	// Bind the PVM
//	//pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
//	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	// Bind the lighting transforms
//	//m_phongShader.bindUniform("ModelMatrix", m_bunnyTransform);
//
//	//m_bunnyMesh.draw();
//#pragma endregion
//
//#pragma region Dragon
//
//	// Bind the PVM
//	//pvm = a_projectionMatrix * a_viewMatrix * m_dragonTransform;
//	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	// Bind the lighting transforms
//	//m_phongShader.bindUniform("ModelMatrix", m_dragonTransform);
//
//	//m_dragonMesh.draw();
//#pragma endregion
//
//#pragma region Buddha
//	// Bind the PVM
//	//pvm = a_projectionMatrix * a_viewMatrix * m_buddhaTransform;
//	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	// Bind the lighting transforms
//	//m_phongShader.bindUniform("ModelMatrix", m_buddhaTransform);
//
//	//m_buddhaMesh.draw();
//#pragma endregion
//
//#pragma region Lucy
//	// Bind the PVM
//	//pvm = a_projectionMatrix * a_viewMatrix * m_lucyTransform;
//	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	// Bind the lighting transforms
//	//m_phongShader.bindUniform("ModelMatrix", m_lucyTransform);
//
//	//m_lucyMesh.draw();
//#pragma endregion
//
//
//#pragma endregion
//
//	std::vector<glm::vec3> lightColours;
//	std::vector<glm::vec3> lightDirs;
//
//	// Separate the direction and colour of each light into their own vectors 
//	// so that we can bind each separately to the shader
//	for (auto& lc : m_lights)
//	{
//		lightColours.push_back(lc.colour);
//		lightDirs.push_back(lc.direction);
//
//	}
//
//#pragma region Soulspear
//	m_normalMapMultiLights.bind();
//	// Bind the transform
//	pvm = a_projectionMatrix * a_viewMatrix * m_spearTransform;
//
//	m_normalMapMultiLights.bindUniform("ProjectionViewModel", pvm);
//
//	m_normalMapMultiLights.bindUniform("CameraPosition", m_cameras[m_currentCamera].GetPosition());
//
//	m_normalMapMultiLights.bindUniform("AmbientColour", m_ambientLight);
//
//	m_normalMapMultiLights.bindUniform("LightColour", (int)lightColours.size(), lightColours.data());
//
//	m_normalMapMultiLights.bindUniform("LightDirection", (int)lightDirs.size(), lightDirs.data());
//
//	m_normalMapMultiLights.bindUniform("lightCount", (int)lightColours.size());
//
//	m_normalMapMultiLights.bindUniform("ModelMatrix", m_spearTransform);
//	// Draw the mesh
//	m_spearMesh.draw();
//
//#pragma endregion
//
//
//#pragma region Sword
//
//	m_normalMapMultiLights.bind();
//	// Bind the transform
//	pvm = a_projectionMatrix * a_viewMatrix * m_swordTransform;
//	m_normalMapMultiLights.bindUniform("ProjectionViewModel", pvm);
//	m_normalMapMultiLights.bindUniform("ModelMatrix", m_spearTransform);
//	m_normalMapMultiLights.bindUniform("AmbientColour", m_ambientLight);
//	m_normalMapMultiLights.bindUniform("LightColour", (int)lightColours.size(), lightColours.data());
//	m_normalMapMultiLights.bindUniform("LightDirection", (int)lightDirs.size(), lightDirs.data());
//	m_normalMapMultiLights.bindUniform("lightCount", (int)lightColours.size());
//	m_normalMapMultiLights.bindUniform("CameraPosition", m_cameras[m_currentCamera].GetPosition());
//	// Draw the mesh
//	m_swordMesh.draw();
//#pragma endregion
//
//
//}

void GraphicsProjectApp::IMGUI_Logic()
{
	// Change light direction and colour in the scene
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -1.f, 1.f);
	ImGui::DragFloat3("Sunlight Colour", &m_scene->GetLight().m_colour[0], 0.1f, 0.f, 2.0f);
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
