#ifndef _globalGameStuff_HG_
#define _globalGameStuff_HG_

//	 _____ _                  ___   ___  _  _ _ _____                   _ _ 
//	|_   _| |_  ___ ___ ___  |   \ / _ \| \| ( )_   _|  _ _  ___ ___ __| (_)
//	  | | | ' \/ -_|_-</ -_) | |) | (_) | .` |/  | |   | ' \/ -_) -_) _` |_ 
//	  |_| |_||_\___/__/\___| |___/ \___/|_|\_|___|_|   |_||_\___\___\__,_(_)
//	 __ _| |__ _ __| |  ___ _ _   / __| |  | __\ \    / /                   
//	/ _` | / _` / _` | / _ \ '_| | (_ | |__| _| \ \/\/ /                    
//	\__, |_\__,_\__,_| \___/_|    \___|____|_|   \_/\_/                     
//	|___/                                                                   
//
// These are files that do NOT require glad or GLFW to be included
// (So be careful what you add here)
//

#include "cGameObject.h"
#include <vector>
#include <glm/vec3.hpp>
#include "cLightManager.h"
#include "cModelAssetLoader.h"
#include "cCamera.h"
#include "Physics/cPhysicsWorld.h"
#include"myBulletPhysics.h"
#include"myPhysics.h"
#include"myInterface.h"

// ******************************************************************
// These require the inclusion of the OpenGL and-or GLFW headers
#include "cVAOMeshManager.h"
#include "cShaderManager.h"
//#include "Texture/cBasicTextureManager.h"
#include "Texture/CTextureManager.h"
#include "cDebugRenderer.h"
// ******************************************************************

// Remember to #include <vector>...
extern std::vector< cGameObject* >  g_vecGameObjects;
extern std::vector< cGameObject* >  g_vecVisualObjects;
extern std::vector< cGameObject* >  g_vecCollisionObjects;
extern std::vector< cGameObject* >  g_vecParticleObjects;
extern std::vector< cGameObject* >  g_vecTaggingObjects;
// Returns 0 or NULL if not found
cGameObject* findObjectByFriendlyName( std::string friendlyName, std::vector<cGameObject*> &vec_pGameObjects );
cGameObject* findObjectByUniqueID( unsigned int ID, std::vector<cGameObject*> &vec_pGameObjects );


extern cLightManager*	g_pLightManager;	// (theMain.cpp)


// Super basic physics update function
//void PhysicsStep( double deltaTime );
extern int blendvar1Sign;
extern float blendvar1;
extern float posBlend;
extern bool HueShiftOn;
extern cPhysicsWorld*	g_pPhysicsWorld;	// (theMain.cpp)
extern float colortime;
extern bool hue;
extern cModelAssetLoader* g_pModelAssetLoader;	// (ModelUtilies.cpp)
extern glm::vec3 windDirection;
extern bool particleEnable;
extern bool shoot;
extern bool tag;
// The teapots
//extern cGameObject* pTP0;
const std::string LEFTTEAPOTNAME = "Left Teapot";		// Now that we have a lookup

extern cCamera* g_pTheCamera;		// (theMain.cpp)

extern cCamera* g_pTheCamera2;

class cGameObject;	// Forward declare to avoid include

extern cVAOMeshManager*			g_pVAOManager;		// (theMain.cpp)
extern cShaderManager*			g_pShaderManager;	// (theMain.cpp)
//extern cBasicTextureManager*	g_pTextureManager;	// (theMain.cpp)
extern CTextureManager*			g_pTextureManager;	// (theMain.cpp)
extern cDebugRenderer*			g_pDebugRenderer;	// (theMain.cpp)

void RenderScene( std::vector< cGameObject* > &vec_pGOs, GLFWwindow* pGLFWWindow, double deltaTime );

extern cGameObject* g_pSkyBoxObject;	// (theMain.cpp)

extern	GLFWwindow* g_pGLFWWindow;	// In TheMain.cpp
extern bool g_IsWindowFullScreen;	// false at start
void setWindowFullScreenOrWindowed( GLFWwindow* pTheWindow, bool IsFullScreen );	// In TheMain.cpp


// Example of skinned mesh.
// NOTE: This is only ONE instance of an loaded FBX file model
extern cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh;			// In ModelUtilites.cpp

// For stencil buffer example...
extern cGameObject* g_Room;						// in theMain.cpp
extern cGameObject* g_RoomMaskForStencil;		// in theMain.cpp
extern std::vector<myInterface::PhysicsObject> PhysicsVector;
extern std::vector<myInterface::force> ForceVector;
extern double deltaTime;

extern float theta;
extern float CAMERAFACTOR;

extern bool W, A, S, D,E, Q,  UP, DOWN, LEFT, RIGHT, SPACE;
extern float jumptime;
extern glm::vec3 oldup;
extern bool up_change;
extern glm::vec3 up;
extern glm::vec3 gravity;
extern std::vector<bool> collision;
extern glm::vec4 respawn;
extern bool changelevel;
extern int level;
extern bool sobelEffect;
#endif
