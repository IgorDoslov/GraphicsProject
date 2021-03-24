#include "Scene.h"
#include "Instance.h"


Scene::Scene(glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight)
	: m_windowSize(a_windowSize), m_light(a_light), m_ambientLight(a_ambientLight),
	m_camera(), m_pointLightColours(), m_pointLightPositions()
{

}

// Iterate over instances and free memory from heap
Scene::~Scene()
{
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		delete(*i);
	}
}

// For adding instances of a model in a scene to a vector
void Scene::AddInstance(Instance* a_instance)
{
	m_instances.push_back(a_instance);
}

// Draw each light and instance in the scene
void Scene::Draw()
{
	// Lights
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].m_direction;
		m_pointLightColours[i] = m_pointLights[i].m_colour;
	}

	// Instances
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		Instance* instance = *i;
		instance->Draw(this);
	}
}
