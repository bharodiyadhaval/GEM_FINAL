#include"Forces.h"

void EngageForces()
{
	myInterface::force forceUP, forceDOWN, forceLEFT, forceRIGHT, forcejump;


	glm::vec3 temp;
	temp = glm::abs ( glm::abs(::up) - 1.0f );


	forceUP.direction = glm::vec3(glm::normalize(::g_vecCollisionObjects[0]->getPosition() - ::g_pTheCamera->getEyePosition()).x * temp.x, glm::normalize(::g_vecCollisionObjects[0]->getPosition() - ::g_pTheCamera->getEyePosition()).y * temp.y, glm::normalize(::g_vecCollisionObjects[0]->getPosition() - ::g_pTheCamera->getEyePosition()).z * temp.z);
	//forceUP.on = 0;
	forceUP.strength = 200;

	forceDOWN.direction = -forceUP.direction;
	
	//forceDOWN.on = 0;
	forceDOWN.strength = 200;

	forceLEFT.direction = glm::normalize(  glm::cross( ::up, forceUP.direction ) );
	//forceLEFT.on = 0;
	forceLEFT.strength = 200;

	forceRIGHT.direction = -forceLEFT.direction;
	//forceRIGHT.on = 0;
	forceRIGHT.strength = 200;


	forcejump.direction = ::up;
	//forceRIGHT.on = 0;
	forcejump.strength = 200;


	::ForceVector.push_back(forceUP);
	::ForceVector.push_back(forceDOWN);
	::ForceVector.push_back(forceLEFT);
	::ForceVector.push_back(forceRIGHT);
	::ForceVector.push_back(forcejump);



}

void UpdateForces()
{
	glm::vec3 temp;
	temp = glm::abs(glm::abs(::up) - 1.0f);


	::ForceVector[0].direction = glm::vec3(glm::normalize(::g_vecCollisionObjects[0]->getPosition() - ::g_pTheCamera->getEyePosition()).x * temp.x, glm::normalize(::g_vecCollisionObjects[0]->getPosition() - ::g_pTheCamera->getEyePosition()).y * temp.y, glm::normalize(::g_vecCollisionObjects[0]->getPosition() - ::g_pTheCamera->getEyePosition()).z * temp.z);


	::ForceVector[1].direction = -::ForceVector[0].direction;
	//forceDOWN.on = 0;

	::ForceVector[2].direction = glm::normalize(glm::cross(::up, ::ForceVector[0].direction));
	//forceLEFT.on = 0;

	::ForceVector[3].direction = -::ForceVector[2].direction;
	//forceRIGHT.on = 0;
}