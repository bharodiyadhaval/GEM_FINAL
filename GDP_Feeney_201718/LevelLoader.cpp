#include "cGameObject.h"
#include <vector>
#include "Utilities.h"
#include <glm/glm.hpp>
#include "globalGameStuff.h"
#include "assimp/cSimpleAssimpSkinnedMeshLoader_OneMesh.h"
#include "cAnimationState.h"



void makeObject
	(glm::vec3 position,float scale, std::string name, bool isReflRefr, float reflect, float refrect, float coeffrefrect, 
	std::string friendlyname, std::string texture1, float text1blend, std::string texture2, float text2blend, bool isphysical, 
	int mass, std::string physicsname, myInterface::PhysicsShapes objecttype)
{
	cGameObject* pTempGO = new cGameObject();
	cPhysicalProperties physState;
	physState.position = position;
	pTempGO->SetPhysState(physState);
	sMeshDrawInfo meshInfo;
	meshInfo.bUseDebugColour = false;
	meshInfo.scale = scale;
	meshInfo.name = name;
	meshInfo.bIsEnvirMapped = isReflRefr;
	if (isReflRefr)
	{
		meshInfo.reflectBlendRatio = reflect;
		meshInfo.refractBlendRatio = refrect;
	}
	
	meshInfo.coefficientRefract = coeffrefrect;
	pTempGO->friendlyName = friendlyname;

	meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo(texture1, text1blend));	
	pTempGO->vecMeshes.push_back(meshInfo);
	myInterface::PhysicsObject tempobj;
	tempobj.ApplyPhysics = isphysical;
	tempobj.Mass = mass;
	tempobj.ObjectName = physicsname;
	tempobj.PhysicsPosition = physState.position;
	tempobj.PhysicsScale = meshInfo.scale;
	tempobj.type = objecttype;
	::PhysicsVector.push_back(tempobj);

	::g_vecCollisionObjects.push_back(pTempGO);		// Fastest way to add

}



void makeObjectnoPhysics
(glm::vec3 position, float scale, std::string name, bool isReflRefr, float reflect, float refrect, float coeffrefrect,
	std::string friendlyname, std::string texture1, float text1blend, std::string texture2, float text2blend, bool isphysical,
	int mass, std::string physicsname, myInterface::PhysicsShapes objecttype)
{
	cGameObject* pTempGO = new cGameObject();
	cPhysicalProperties physState;
	physState.position = position;
	pTempGO->SetPhysState(physState);
	sMeshDrawInfo meshInfo;
	meshInfo.bUseDebugColour = false;
	meshInfo.scale = scale;
	meshInfo.name = name;
	meshInfo.bIsEnvirMapped = isReflRefr;
	if (isReflRefr)
	{
		meshInfo.reflectBlendRatio = reflect;
		meshInfo.refractBlendRatio = refrect;
	}

	meshInfo.coefficientRefract = coeffrefrect;
	pTempGO->friendlyName = friendlyname;

	meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo(texture1, text1blend));
	pTempGO->vecMeshes.push_back(meshInfo);
	myInterface::PhysicsObject tempobj;
	tempobj.ApplyPhysics = isphysical;
	tempobj.Mass = mass;
	tempobj.ObjectName = physicsname;
	tempobj.PhysicsPosition = physState.position;
	tempobj.PhysicsScale = meshInfo.scale;
	tempobj.type = objecttype;
	//::PhysicsVector.push_back(tempobj);

	::g_vecTaggingObjects.push_back(pTempGO);		// Fastest way to add

}

void LoadGameLevel(int level, bool &changelevel)
{
	if (changelevel == true)
	{


		::g_vecCollisionObjects.clear();
		::PhysicsVector.clear();
		switch (level)
		{
		case 1:
			////////////////////////////////////////////////////////////////////////////////
			makeObject
			(glm::vec3(0, 300, 0), 15.0f, "technosphere", true, 0.2f, 0.6f, 0.5,
				"ball", "Iron_Plate/Iron_Plate_SPC.bmp", 0.1f, "", 0, true,
				50, "Ball", myInterface::SPHERE);

			for (int i = 0; i < 9; i++)
			{
				if (i == 1 || i==2)
				{
					makeObject
					(glm::vec3(0, 0, 300 * (i*1.2)), 100.0f, "Metal_Plate", false, 0, 0, 0,
						"plate", "Iron_Plate/Iron_Plate_SPC.bmp", 1.0f, "", 0, true,
						50, "Metal_Box", myInterface::BOX);
				}

				else
				{
					makeObject
					(glm::vec3(0, 0, 300 * (i*1.2)), 100.0f, "Metal_Plate", false, 0, 0, 0,
						"plate", "Iron_Plate/Iron_Plate_SPC.bmp", 1.0f, "", 0, true,
						0, "Metal_Box", myInterface::BOX);
				}
			


			}
		/*	makeObject
			(glm::vec3(0, 200, 400), 2.0f, "screen", false, 0.0f, 0.0f, 0.0,
				"portal", "Iron_Plate/Iron_Plate_SPC.bmp", 1.0f, "", 0, true,
				0, "portal", myInterface::SPHERE);
*/
			for (int i = 0; i <= 15; i++)
			{
				makeObjectnoPhysics
				(glm::vec3(rand()%1500 - 300, rand() % 400 - 200, rand() % 1500 - 300), 15.0f, "technosphere", true, 0.2f, 0.6f, 0.5,
					"ball", "Iron_Plate/Iron_Plate_SPC.bmp", 0.1f, "", 0, true,
					50, "Ball", myInterface::SPHERE);
			}

			::g_vecTaggingObjects[0]->vecMeshes[0].bDrawAsWireFrame = 1;
			::g_vecTaggingObjects[0]->vecMeshes[0].scale = 10.0f;
















			////////////////////////////////////////////////////////////////////////////////

			break;

		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
		}(level);




	}
	changelevel = 0;
}