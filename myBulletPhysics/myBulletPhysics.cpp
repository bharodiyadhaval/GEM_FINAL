#include"myBulletPhysics.h"

void myBulletPhysics::ReadFileToToken(std::ifstream &file, std::string token)
{
	bool bKeepReading = true;
	std::string garbage;
	do
	{
		file >> garbage;		// Title_End??
		if (garbage == token)
		{
			return;
		}
	} while (bKeepReading);
	return;
}

int bugfix = 0;

void myBulletPhysics::InitializePhysics(std::vector<PhysicsObject> &PhysicsVector, glm::vec3 &gravity, std::vector<bool> &collision, int level)
{

	//Physics Bullet Initializer

	
	dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////GROUND SHAPE DEFINITION
	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	//btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -2, 0)));
	//btRigidBody::btRigidBodyConstructionInfo  groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	//btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	//dynamicsWorld->addRigidBody(groundRigidBody);


	////BACKWALL SHAPE DEFINITION
	//btCollisionShape* BackWallShape = new btStaticPlaneShape(btVector3(0, 0, 1), 0);
	//btDefaultMotionState* BackWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -14)));
	//btRigidBody::btRigidBodyConstructionInfo  BackWallRigidBodyCI(0, BackWallMotionState, BackWallShape, btVector3(0, 0, 0));
	//btRigidBody* BackWallRigidBody = new btRigidBody(BackWallRigidBodyCI);
	//dynamicsWorld->addRigidBody(BackWallRigidBody);

	////LeftWall SHAPE DEFINITION
	//btCollisionShape* LeftWallShape = new btStaticPlaneShape(btVector3(1, 0, 0), 0);
	//btDefaultMotionState* LeftWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-14, 0, 0)));
	//btRigidBody::btRigidBodyConstructionInfo  LeftWallRigidBodyCI(0, LeftWallMotionState, LeftWallShape, btVector3(0, 0, 0));
	//btRigidBody* LeftWallRigidBody = new btRigidBody(LeftWallRigidBodyCI);
	//dynamicsWorld->addRigidBody(LeftWallRigidBody);

	////RightWall SHAPE DEFINITION
	//btCollisionShape* RightWallShape = new btStaticPlaneShape(btVector3(-1, 0, 0), 0);
	//btDefaultMotionState* RightWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(16, 0, 0)));
	//btRigidBody::btRigidBodyConstructionInfo  RightWallRigidBodyCI(0, RightWallMotionState, RightWallShape, btVector3(0, 0, 0));
	//btRigidBody* RightWallRigidBody = new btRigidBody(RightWallRigidBodyCI);
	//dynamicsWorld->addRigidBody(RightWallRigidBody);

	////FRONTWALL SHAPE DEFINITION
	//btCollisionShape* FrontWallShape = new btStaticPlaneShape(btVector3(0, 0, -1), 0);
	//btDefaultMotionState* FrontWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 16)));
	//btRigidBody::btRigidBodyConstructionInfo  FrontWallRigidBodyCI(0, FrontWallMotionState, FrontWallShape, btVector3(0, 0, 0));
	//btRigidBody* FrontWallRigidBody = new btRigidBody(FrontWallRigidBodyCI);
	//dynamicsWorld->addRigidBody(FrontWallRigidBody);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < BulletRigidBodies.size(); i++)
	{
		dynamicsWorld->removeRigidBody(BulletRigidBodies[i]);


	}
	BulletRigidBodies.clear();
	BulletObjectShapes.clear();
	BulletMotionStates.clear();
	BulletObjectMasses.clear();
	BulletObjectIntertias.clear();
	collision.clear();
	std::vector <PhysicsObject> myvec;
	


	std::cout << "Inside Init Physics" << std::endl;

	for (int i = 0; i < PhysicsVector.size(); i++)
	{
		collision.push_back(0);
		switch (PhysicsVector[i].type)
		{

		case PhysicsShapes::BOX :

			BulletObjectMasses.push_back(PhysicsVector[i].Mass);
			BulletObjectShapes.push_back(new btBoxShape( btVector3(PhysicsVector[i].PhysicsScale, PhysicsVector[i].PhysicsScale, PhysicsVector[i].PhysicsScale ) ) );

			break;

		case PhysicsShapes::SPHERE:

			BulletObjectMasses.push_back(PhysicsVector[i].Mass);
			BulletObjectShapes.push_back( new btSphereShape ( PhysicsVector[i].PhysicsScale ) );

			break;


		case PhysicsShapes::PLANE:

			BulletObjectMasses.push_back(0);
			BulletObjectShapes.push_back(new btStaticPlaneShape(btVector3(0, 1, 0), 1));

			break;

		default:
			break;
		}
		
		BulletMotionStates.push_back(	new btDefaultMotionState (	 (	btTransform	(	btQuaternion	(	0, 0, 0, 1	)	, btVector3	(	 PhysicsVector[i].PhysicsPosition.x ,
																																			 PhysicsVector[i].PhysicsPosition.y, 
																																	         PhysicsVector[i].PhysicsPosition.z	 ) ) ) ) );
		BulletObjectIntertias.push_back(btVector3(0,0,0));

	}



	for (int i = 0; i < PhysicsVector.size(); i++)
	{
		std::cout << "Inside RigidBodyGen" << std::endl;
		BulletObjectShapes[i]->calculateLocalInertia(BulletObjectMasses[i], BulletObjectIntertias[i]);
		BulletRigidBodies.push_back(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(BulletObjectMasses[i], BulletMotionStates[i], BulletObjectShapes[i], BulletObjectIntertias[i])));

	}

	for (int i = 0; i < BulletRigidBodies.size(); i++)
	{
		dynamicsWorld->addRigidBody(BulletRigidBodies[i]);

	}



	//////////////////////////////////////////////////////
	//INITIALIZE
	//LEVEL 1 CONSTRAINTS
	if (level == 1)
	{
	
		//btSliderConstraint Tile1Slide = new btSliderConstraint(BulletRigidBodies[1],)
		btTransform sliderT;
		sliderT = btTransform::getIdentity();
		//btSliderConstraint* SlideTheBlock2 = new btSliderConstraint(*rigidbodies[6], sliderT2, false);

		btGeneric6DofConstraint* sixDOF = new btGeneric6DofConstraint(*BulletRigidBodies[2], sliderT, false);
		sixDOF->setLinearLowerLimit(btVector3(-500, 0, 0));
		sixDOF->setAngularLowerLimit(btVector3(0, 0, 0));
		sixDOF->setAngularUpperLimit(btVector3(0, 0, 0));
		sixDOF->setLinearUpperLimit(btVector3(500, 0, 0));
		btGeneric6DofConstraint* sixDOF2 = new btGeneric6DofConstraint(*BulletRigidBodies[3], sliderT, false);
		sixDOF2->setLinearLowerLimit(btVector3(-500, 0, 0));
		sixDOF2->setAngularLowerLimit(btVector3(0, 0, 0));
		sixDOF2->setAngularUpperLimit(btVector3(0, 0, 0));
		sixDOF2->setLinearUpperLimit(btVector3(500, 0, 0));

		BulletRigidBodies[2]->setGravity(btVector3(0, 0, 0));
		BulletRigidBodies[3]->setGravity(btVector3(0, 0, 0));


		dynamicsWorld->addConstraint(sixDOF, false);
		dynamicsWorld->addConstraint(sixDOF2, false);



	}
	







	return;
}






void myBulletPhysics::PhysicsStep(double deltaTime, std::vector < force > &PhysicsForces, std::vector<PhysicsObject> &PhysicsVector, glm::vec3 &gravity, int InteractObj, std::vector<bool> &collision, glm::vec4 &respawn, int level)
{


	dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));

	
	dynamicsWorld->stepSimulation(deltaTime, 500);
	
	
	btTransform trans;
	btQuaternion orient;
	btTransform trans2;

	float xr, yr, zy;

	

	if (bugfix == 0)
	{
		BulletRigidBodies[0]->applyCentralImpulse(0.5 * btVector3(10,
			10,
			10));
	}
	for (int i = 0; i < BulletRigidBodies.size(); i++)
	{
		BulletRigidBodies[i]->getMotionState()->getWorldTransform(trans);
		orient = trans.getRotation();
		
		if (i == InteractObj)
		{
			for (int j = 0; j < PhysicsForces.size(); j++)
			{
				if (PhysicsForces[j].on)
				{
					std::cout << "Force" << j << " is Engaged" << std::endl;

					if (j==4)
					{
						
						btVector3 Cimp = (float)PhysicsForces[j].strength * btVector3(-gravity.x / 4, -gravity.y / 4, -gravity.z / 4);
						BulletRigidBodies[i]->applyCentralImpulse(Cimp);
						PhysicsForces[j].on = 0;
					}
					
					BulletRigidBodies[i]->applyCentralImpulse( (float)PhysicsForces[j].strength * btVector3( PhysicsForces[j].direction.x, 
																											 PhysicsForces[j].direction.y,
																											 PhysicsForces[j].direction.z  )  );
				}
			}

		}

		
		PhysicsVector[i].PhysicsPosition = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		//std::cout << PhysicsVector[0].PhysicsPosition.y<<std::endl;
		PhysicsVector[i].PhysicsOrientation2 = glm::quat( orient.getW(), orient.getX(), orient.getY(), orient.getZ());
		
		if (i == 0)
		{
			if (respawn[0] == 1)
			{
				btTransform newPosition;
				newPosition.setOrigin(btVector3(respawn[1], respawn[2], respawn[3]));
				BulletRigidBodies[0]->setMotionState(new btDefaultMotionState((btTransform(btQuaternion(0, 0, 0, 1), btVector3(respawn[1], respawn[2], respawn[3])))));
				//BulletRigidBodies[0]->setWorldTransform(newPosition);

				PhysicsVector[0].PhysicsPosition = glm::vec3(respawn[1], respawn[2], respawn[3]);
				BulletRigidBodies[0]->clearForces();
				BulletRigidBodies[0]->setLinearVelocity(btVector3(0, 0, 0));
				respawn[0] = 0;

			}
		}
		
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	btTransform Motionstatevar;
	switch (level)
	{
	case 1:

		float force = 20.0f;	
		//LEVEL 1 CONSTRAINTS AND OBSTACLES
	//	BulletRigidBodies[1]->isKinematicObject();
	//	BulletRigidBodies[1]->setMassProps(50, btVector3(0, 0, 0));
		BulletRigidBodies[2]->getMotionState()->getWorldTransform(Motionstatevar);
		if ( Motionstatevar.getOrigin().getX() >= 250 )
		{
			force = -abs(force);
		}
		if( Motionstatevar.getOrigin().getX() <= -250)
		{
			force = abs(force);
		}
		BulletRigidBodies[2]->applyCentralForce(btVector3(force*200, 0, 0));
		BulletRigidBodies[3]->applyCentralForce(btVector3(-force * 200, 0, 0));

	
		
		
		
		
		break;

	}



















	return;
}



