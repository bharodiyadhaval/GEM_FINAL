#pragma once
#include <btBulletDynamicsCommon.h>
#include<btBulletCollisionCommon.h>
#include"myInterface.h"
#include <globalOpenGL_GLFW.h>
//#include<vector>
class myBulletPhysics : public myInterface
{
public:

	virtual void ReadFileToToken(std::ifstream &file, std::string token);

	virtual void InitializePhysics(std::vector<PhysicsObject> &PhysicsVector, glm::vec3 &gravity, std::vector<bool> &collision, int level);

	virtual void PhysicsStep(double deltaTime, std::vector< force > &PhysicsForces, std::vector<PhysicsObject> &PhysicsVector, glm::vec3 &gravity, int InteractObj, std::vector<bool> &collision, glm::vec4 &respawn, int level);
	std::vector<btRigidBody*> BulletRigidBodies;
private:

	std::vector <btCollisionShape*> BulletObjectShapes;

	

	std::vector<btDefaultMotionState*> BulletMotionStates;

	std::vector<btScalar> BulletObjectMasses;

	std::vector<btVector3> BulletObjectIntertias;


	btBroadphaseInterface* BulletBroadPhase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, BulletBroadPhase, solver, collisionConfiguration);


	
};



