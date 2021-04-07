/*
Author:		Igor Doslov
Date:       7/4/2021
File:		Instance.h
Purpose:	To create an instance of a model
*/
#pragma once
#include <glm/glm.hpp>

class Camera;
class Scene;

namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{
public:
	Instance(glm::mat4 a_transform, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);
	Instance(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);
	
	void Draw(Scene* a_scene);

	static glm::mat4 MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale);

	// Position, rotaion and scale vectors to be manipulated in IMGUI_Logic()
	glm::vec3 m_pos = glm::vec3(0); // position
	glm::vec3 m_rot = glm::vec3(0); // rotation
	glm::vec3 m_scale = glm::vec3(1); // scale

	// Called in IMGUI_Logic() for each instance of a model so its position, rotation and scale can be changed
	void RecalculateTransform() { m_transform = MakeTransform(m_pos, m_rot, m_scale); }


	glm::mat4 m_transform;
protected:
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;

};

