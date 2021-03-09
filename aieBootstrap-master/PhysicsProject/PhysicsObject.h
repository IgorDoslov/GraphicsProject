#pragma once
#include <glm/glm.hpp>

// This is a pure abstract class. It will be used as a
// base for the derived classes that will use FixedUpdate

enum ShapeType {
	JOINT = -1,
	PLANE = 0,
	SPHERE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
public:
	PhysicsObject();
	~PhysicsObject();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep) = 0;
	
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	virtual void ResetPosition() {};

	virtual bool IsInside(glm::vec2 a_point) = 0;

	ShapeType GetShapeID() { return m_shapeID; }

	bool IsKinematic() { return m_isKinematic; }
	bool SetKinematic(bool a_state) { return m_isKinematic = a_state; }

	glm::vec2 GetPosition() const { return m_physObjPosition; }
	glm::vec2 SetPosition(glm::vec2 a_newPos) { return m_physObjPosition = a_newPos; }

	float GetElasticity() { return m_elasticity; }
	// Keep to 1 for realism
	float SetElasticity(float a_new) { return m_elasticity = a_new; }

protected:
	ShapeType m_shapeID;
	bool m_isKinematic = false;
	float m_elasticity = 0.f;

	glm::vec2 m_physObjPosition = glm::vec2(0);
	

	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID){}
};

