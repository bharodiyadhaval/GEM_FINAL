#pragma once
#include"PhysicsDefinitions.h"

class myInterface : public PhysicsDefinitions
{
public: 
	
	virtual ~myInterface() {}

	virtual void ReadFileToToken(std::ifstream &file, std::string token) = 0;

	virtual void InitializePhysics(std::vector<PhysicsObject> &PhysicsVector, glm::vec3 &gravity, std::vector<bool> &collision, int level) = 0;

	virtual void PhysicsStep(double deltaTime, std::vector< force > &PhysicsForces, std::vector<PhysicsObject> &PhysicsVector, glm::vec3 &gravit,  int InteractObj, std::vector<bool> &collision, glm::vec4 &respawn, int level) = 0;

}; 