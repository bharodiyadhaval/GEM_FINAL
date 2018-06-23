#pragma once

#include"globalGameStuff.h"
#include<time.h>
class ParticleSystem
{

public:



	

	enum particleType
	{
		FIRE,
		SMOKE,
		FOUNTAIN,
		EXPLODE,
		RAIN,
		STARWARS,
		TORNADO
	};
	particleType GeneratorType;
	
	glm::vec3 genLocation;
	glm::vec3 GenposChange[2];
	int poschangenumber = 0;
	glm::vec3 wind = glm::vec3(0,0,0);
	glm::vec4 Birthcolor;
	double LastgenTime=0;
	double particleAge;
	glm::vec3 smokeWind = glm::vec3(0, 0, 0);
	void createParticle(int spread, float size, int pollRate, int numPerSec);
	void updateParticles(float speed);
	void deleteOldParticles( float age);
	bool bEnableSmokePositionBasedMotion = 0;
	void EnableSmokePositionBasedMotion();
	void initParticles(glm::vec3 DEFgenlocation);
};


void ParticleSystem::createParticle(int spread, float size, int pollRate, int numPerSec)
{

	if(LastgenTime + pollRate <= glfwGetTime())
	{ 
		
		LastgenTime = glfwGetTime();
		for (int i = 0; i < numPerSec; i++)
		{
			cGameObject* pTempGO = new cGameObject();
			cPhysicalProperties physState;
			switch (this->GeneratorType)
			{
			case SMOKE:
				this->Birthcolor = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
				break;
			case FIRE:
				this->Birthcolor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

				break;
			case EXPLODE:
				break;
			case RAIN:
				break;
			case STARWARS:
				break;
			case TORNADO:
				this->Birthcolor = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);

				break;

			}
			physState.position = this->genLocation + glm::vec3((rand() % spread - spread / 2), (rand() % spread - spread / 2), (rand() % spread - spread / 2));
			physState.rigidBodyShape = physState.SPHERE;

			physState.rigidBodyMeasurements.sphere_capsule_radius = size;
			pTempGO->SetPhysState(physState);
			sMeshDrawInfo meshInfo;
			meshInfo.bUseDebugColour = 1;
			meshInfo.debugDiffuseColour = this->Birthcolor;
			meshInfo.bDrawAsWireFrame = 1;
			meshInfo.scale = size;
			meshInfo.name = "Piramid";
			meshInfo.bIsEnvirMapped = false;
			meshInfo.reflectBlendRatio = 0.2f;
			meshInfo.refractBlendRatio = 0.6f;
			meshInfo.coefficientRefract = 0.5;
			pTempGO->friendlyName = "Piramid_Particle";
			pTempGO->vecMeshes.push_back(meshInfo);

			pTempGO->particleTime = glfwGetTime();

			::g_vecParticleObjects.push_back(pTempGO);
		}
		
	}
	

}

void ParticleSystem::updateParticles( float speed)
{
	switch (this->GeneratorType)
	{
	case SMOKE:
		for (int i = 0; i < ::g_vecParticleObjects.size(); i++)
		{
			float distance = glm::distance( ::g_vecParticleObjects[i]->getPosition(), this->genLocation);

			glm::vec3 upwardDisplace = ::up * 35.0f;
			glm::vec3 curPos = ::g_vecParticleObjects[i]->getPosition();
			glm::vec3 spatialDisplace = glm::vec3((1 - ::up.x)*( curPos.x - genLocation.x), (1 - ::up.y)*(curPos.y - genLocation.y), (1 - ::up.z)*(curPos.z - genLocation.z));
			glm::vec3 movementDisplace = glm::normalize(GenposChange[0] - GenposChange[1]);
			

			if (this->bEnableSmokePositionBasedMotion)
			{
				::g_vecParticleObjects[i]->overwritePotition(curPos + (upwardDisplace + spatialDisplace + wind * distance / 10.0f + movementDisplace * distance / 10.0f) * (float) ::deltaTime * speed);

			}
			else
			{
				::g_vecParticleObjects[i]->overwritePotition(curPos + (upwardDisplace + spatialDisplace + wind * distance / 10.0f ) * (float) ::deltaTime * speed);

			}

			if (glfwGetTime() >= ::g_vecParticleObjects[i]->particleTime + particleAge - 1 && ::g_vecParticleObjects[i]->vecMeshes[0].debugDiffuseColour.a>=0)
			{
				::g_vecParticleObjects[i]->vecMeshes[0].debugDiffuseColour.a -= (float)deltaTime;

			}
		}
		break;
	case FIRE:
		for (int i = 0; i < ::g_vecParticleObjects.size(); i++)
		{
			float distance = glm::distance(::g_vecParticleObjects[i]->getPosition(), this->genLocation);

			glm::vec3 upwardDisplace = ::up * 35.0f;
			glm::vec3 curPos = ::g_vecParticleObjects[i]->getPosition();
			glm::vec3 spatialDisplace = glm::vec3((1 - ::up.x)*(curPos.x - genLocation.x), (1 - ::up.y)*(curPos.y - genLocation.y), (1 - ::up.z)*(curPos.z - genLocation.z));
			glm::vec3 movementDisplace = glm::normalize(GenposChange[0] - GenposChange[1]);


		
				::g_vecParticleObjects[i]->overwritePotition(curPos + (upwardDisplace + spatialDisplace + wind * distance / 10.0f) * (float) ::deltaTime * speed);

			

			if (glfwGetTime() >= ::g_vecParticleObjects[i]->particleTime + particleAge - 1 && ::g_vecParticleObjects[i]->vecMeshes[0].debugDiffuseColour.a >= 0)
			{
				::g_vecParticleObjects[i]->vecMeshes[0].debugDiffuseColour.a -= (float)deltaTime;

			}
		}
		break;
	case EXPLODE:
		break;
	case RAIN:
		break;
	case STARWARS:
		for (int i = 0; i < ::g_vecParticleObjects.size(); i++)
		{
			::g_vecParticleObjects[i]->overwritePotition(::g_vecParticleObjects[i]->getPosition() + (::g_vecParticleObjects[i]->getPosition() - genLocation) * (float) ::deltaTime * speed);

			//::g_vecParticleObjects[i]->vecMeshes[0].debugDiffuseColour += (glm::vec4(0 g, b, a) - Birthcolor) *1.3f / (float)particleAge * (float)deltaTime;
		}
		break;
	case TORNADO:
		for (int i = 0; i < ::g_vecParticleObjects.size(); i++)
		{
			float distance = glm::distance(::g_vecParticleObjects[i]->getPosition(), this->genLocation)/5.0f;
			float yDist = ::g_vecParticleObjects[i]->getPosition().y - this->genLocation.y;
			::g_vecParticleObjects[i]->overwritePotition((glm::vec3(::g_vecParticleObjects[i]->getPosition().x + yDist*5.0f *cos(::g_vecParticleObjects[i]->getPosition().y) * ::deltaTime, ::g_vecParticleObjects[i]->getPosition().y + 20.0f*(float) ::deltaTime * speed, ::g_vecParticleObjects[i]->getPosition().z + yDist*5.0f*sin(::g_vecParticleObjects[i]->getPosition().y) *::deltaTime  )) );

			//::g_vecParticleObjects[i]->vecMeshes[0].debugDiffuseColour += (glm::vec4(0 g, b, a) - Birthcolor) *1.3f / (float)particleAge * (float)deltaTime;
		}
		break;

	}
		

	
}

void ParticleSystem::deleteOldParticles( float age )
{
	particleAge = age;

	

		for (int i = 0; i < ::g_vecParticleObjects.size(); i++)
		{
			if (::g_vecParticleObjects[i]->particleTime + age <= glfwGetTime())
			{
				::g_vecParticleObjects.erase(::g_vecParticleObjects.begin() + i);
			}
		}
	
	

	std::cout << "\n=======\nParticles Generated On Screen::\n" << ::g_vecParticleObjects.size() << "\n=========================\n" << std::endl;
}

void ParticleSystem::initParticles(glm::vec3 DEFgenlocation)
{
	this->genLocation = DEFgenlocation;
	this->GenposChange[0] = this->genLocation;
	this->GenposChange[1] = this->genLocation;

}

void ParticleSystem::EnableSmokePositionBasedMotion()
{
	this->bEnableSmokePositionBasedMotion = 1;
	if (this->poschangenumber == 0)
	{
		this->GenposChange[this->poschangenumber] = this->genLocation;
		this->poschangenumber++;

	}
	else if (poschangenumber == 1)
	{
		this->GenposChange[this->poschangenumber] = this->genLocation;
		this->poschangenumber = 0;
	}
	else
	{
		this->poschangenumber = 0;

	}
}