// Include glad and GLFW in correct order
#include "globalOpenGL_GLFW.h"


#include <iostream>			// C++ cin, cout, etc.
//#include "linmath.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


#include <stdlib.h>
#include <stdio.h>
// Add the file stuff library (file stream>
#include <fstream>
#include <sstream>		// "String stream"
#include <string>

#include <vector>		//  smart array, "array" in most languages
#include "Utilities.h"
#include "ModelUtilities.h"
#include "cMesh.h"
#include "sMeshDrawInfo.h"
#include "cShaderManager.h" 
#include "cGameObject.h"
#include "cVAOMeshManager.h"


//#include "Physics/Physics.h"	// Physics collision detection functions
#include "Physics/cPhysicsWorld.h"

#include "cLightManager.h"

// Include all the things that are accessed in other files
#include "globalGameStuff.h"

#include "cCamera.h"

#include "cFBO.h" 

//**********
// BE a little careful of including this everywhere...
#include "assimp/cSimpleAssimpSkinnedMeshLoader_OneMesh.h"
//**********

//#include "Dalek_Threaded_01.h"
//#include "Dalek_Threaded_02.h"
#include "Dalek_Threaded_03.h"
#include "cDalek.h"
#include"Forces.h"
#include"SoundEngine.h"
#include"ParticleSystem.h"



float blendvar1 = 0;
int blendvar1Sign = 1;
float posBlend = 0;
glm::vec3 windDirection = glm::vec3(-1,0,-1);
float colortime = 0;
bool hue = true;
bool sobelEffect = 0;
bool HueShiftOn = 0;
void LoadGameLevel(int level, bool &changelevel);
int exitwhile = 0;
glm::vec3 oldup = glm::vec3(0, 1, 0);
bool up_change=0;
float theta = 180;
float CAMERAFACTOR = 80.0f;
float vertDistance = 150.0f; 
bool jumplock = 0;
float jumptime = 0;
float jumpcontrol = 0;
bool particleEnable = 0;
bool W =0, A = 0, S = 0, D = 0, E = 0, Q = 0, UP = 0, DOWN = 0, LEFT = 0, RIGHT = 0, SPACE = 0;
std::vector<bool> collision;
glm::vec4 respawn = glm::vec4(0,0,0,0);
std::vector<myInterface::PhysicsObject> PhysicsVector;
std::vector<myInterface::force> ForceVector;
glm::vec3 up = glm::vec3(0,1,0);
glm::vec3 gravity = ::up* (float)(-200);
int level = 0;
double deltaTime;
bool changelevel = 0;
SoundEngine* Audio = new SoundEngine();
iDalekManager* g_pDalekManager; 

glm::vec3 ray;

int particleDensity = 1;


// Here, the scene is rendered in 3 passes:
// 1. Render geometry to G buffer
// 2. Perform deferred pass, rendering to Deferred buffer
// 3. Then post-pass ("2nd pass" to the scree)
//    Copying from the Pass2_Deferred buffer to the final screen
cFBO pass1_cFBO;
cFBO pass2_cFBO;


void DrawDebugLightingSpheres(void);


cGameObject* g_pSkyBoxObject = NULL;	// (theMain.cpp)


// Remember to #include <vector>...
std::vector< cGameObject* >  g_vecGameObjects;
std::vector< cGameObject* >  g_vecVisualObjects;
std::vector< cGameObject* >  g_vecCollisionObjects;
std::vector< cGameObject* >  g_vecParticleObjects;
std::vector< cGameObject* >  g_vecTaggingObjects;
ParticleSystem* ParticleSystem1 = new ParticleSystem();



bool shoot=0;
bool tag=0;


// Used for the light pass of the deferred rendering
cGameObject* g_pLightSphere2Sided = NULL;
cCamera* g_pTheCamera = NULL;
cCamera* g_pTheCamera2 = NULL;

cVAOMeshManager* g_pVAOManager = 0;		// or NULL, or nullptr

cShaderManager*		g_pShaderManager = 0;		// Heap, new (and delete)
cLightManager*		g_pLightManager = 0;

CTextureManager*		g_pTextureManager = 0;

cModelAssetLoader*		g_pModelAssetLoader = NULL;


cDebugRenderer*			g_pDebugRenderer = 0;


// This contains the AABB grid for the terrain...
cAABBBroadPhase* g_terrainAABBBroadPhase = 0;

cPhysicsWorld*	g_pPhysicsWorld = NULL;	// (theMain.cpp)

// This is used in the "render a previous pass" object
cGameObject* g_ExampleTexturedQuad = NULL;

// For stencil buffer example...
cGameObject* g_Room = NULL;
cGameObject* g_RoomMaskForStencil = NULL;



#include "cFrameBuffer.h"

bool g_IsWindowFullScreen = false;
GLFWwindow* g_pGLFWWindow = NULL;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


float g_ChromaticAberrationOffset = 0.0f;
float g_CR_Max = 0.1f;
double g_CA_CountDown = 0.0f;

const int PassInt0 = 0;
const int PassInt1 = 1;
const int PassInt2 = 2;


// Moved to GLFW_keyboardCallback.cpp
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)










void SmoothKeys(float deltaTime)
{
	
	if (::Q == 1)
	{
		vertDistance += 15 * deltaTime;
		particleDensity *= 10;
	}

	if (::E == 1)
	{
		vertDistance -= 15 * deltaTime;

	}
	
	if (::W == 1)
	{
		::ForceVector[0].on = 1;

	}
	else
	{
		::ForceVector[0].on = 0;

	}

	if (::A == 1)
	{
		::ForceVector[2].on = 1;

	}
	else
	{
		::ForceVector[2].on = 0;

	}


	if (::S == 1)
	{
		::ForceVector[1].on = 1;

	}
	else
	{
		::ForceVector[1].on = 0;
	}
	if (::D == 1)
	{
		::ForceVector[3].on = 1;
	}
	else
	{
		::ForceVector[3].on = 0;
	}

	if (::UP == 1)
	{
		CAMERAFACTOR+=40*deltaTime;
	}

	if (::DOWN== 1)
	{
		CAMERAFACTOR -= 40*deltaTime;

	}

	if (::LEFT == 1)
	{
		theta += 5*deltaTime;

	}
	if (::RIGHT == 1)
	{
		theta -= 5*deltaTime;
	}

	if (::SPACE == 1 && jumplock ==0)
	{
		::ForceVector[4].on = 1;
		ray = g_vecCollisionObjects[0]->getPosition();
		Audio->SoundSystem->playSound(Audio->Sounds[2], 0, false, &Audio->ChannelList[2]);
		//jumpcontrol = glfwGetTime();
		//jumplock = 1;
	}
	else
	{
		::ForceVector[4].on = 0;
	}
	
	
}



























void setUpTheCamera()
{
	if (::up.x == 1)
	{
		::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x + 3.5*CAMERAFACTOR, ::g_vecCollisionObjects[0]->getPosition().y + (CAMERAFACTOR + vertDistance) * cos(theta), ::g_vecCollisionObjects[0]->getPosition().z + (CAMERAFACTOR + vertDistance) * sin(theta)));

	}
	else if (::up.y == 1)
	{
		::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x + (CAMERAFACTOR + vertDistance) * cosf(theta), ::g_vecCollisionObjects[0]->getPosition().y + 3.5*CAMERAFACTOR, ::g_vecCollisionObjects[0]->getPosition().z + (CAMERAFACTOR + vertDistance) * sin(theta)));

	}
	else if (::up.z == 1)
	{
		::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x + (CAMERAFACTOR + vertDistance) * cosf(theta), ::g_vecCollisionObjects[0]->getPosition().y + (CAMERAFACTOR + vertDistance) * sin(theta), ::g_vecCollisionObjects[0]->getPosition().z + 3.5*CAMERAFACTOR));

	}
	else if (::up.x == -1)
	{
		::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x - 1.5*CAMERAFACTOR, ::g_vecCollisionObjects[0]->getPosition().y + (CAMERAFACTOR + vertDistance) * sin(theta), ::g_vecCollisionObjects[0]->getPosition().z + (CAMERAFACTOR + vertDistance) * cos(theta)));

	}
	else if (::up.y == -1)
	{
		::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x + (CAMERAFACTOR + vertDistance) * sin(theta), ::g_vecCollisionObjects[0]->getPosition().y - 1.5*CAMERAFACTOR, ::g_vecCollisionObjects[0]->getPosition().z + (CAMERAFACTOR + vertDistance) * cos(theta)));

	}
	else if (::up.z == -1)
	{
		::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x + (CAMERAFACTOR + vertDistance) * sin(theta), ::g_vecCollisionObjects[0]->getPosition().y + (CAMERAFACTOR + vertDistance) * cos(theta), ::g_vecCollisionObjects[0]->getPosition().z - 1.5*CAMERAFACTOR));

	}
	else
	{

	}
}












































int main(void)
{

	srand(time(NULL));
	ParticleSystem1->initParticles(glm::vec3(0, 50, 200));

	::sobelEffect = 0;
	::jumptime = 0;

	glm::vec3 gravitychange = ::up;

	myInterface* PhysicsHandler1 = new myBulletPhysics();
	
	Audio->LoadFiles();
	Audio->Initializer();
	Audio->ErrorCheck(Audio->OutputResult);

	std::cout << "Select the Compressed/Uncompressed Version for the Streaming Sound:\n";
	std::cout << "Compressed/Uncompressed 1/2 :";
	int selector;
	std::cin >> selector;


	if (selector == 1)
	{
		Audio->LoadCompressedStreams();
	}
	else
	{
		Audio->LoadUncompressedStreams();
	}

	Audio->LoadSampledSounds();

	//GLFWwindow* pGLFWWindow;		// Moved to allow switch from windowed to full-screen
	glfwSetErrorCallback(error_callback);



    if (!glfwInit())
	{
		// exit(EXIT_FAILURE);
		std::cout << "ERROR: Couldn't init GLFW, so we're pretty much stuck; do you have OpenGL??" << std::endl;
		return -1;
	}

	int height = 480;	/* default */
	int width = 640;	// default
	std::string title = "OpenGL Rocks";

	std::ifstream infoFile("config.txt");
	if ( ! infoFile.is_open() )
	{	// File didn't open...
		std::cout << "Can't find config file" << std::endl;
		std::cout << "Using defaults" << std::endl;
	}   
	else
	{	// File DID open, so read it... 
		std::string a;	
		
		infoFile >> a;	// "Game"	//std::cin >> a;
		infoFile >> a;	// "Config"
		infoFile >> a;	// "width"

		infoFile >> width;	// 1080

		infoFile >> a;	// "height"

		infoFile >> height;	// 768

		infoFile >> a;		// Title_Start

		std::stringstream ssTitle;		// Inside "sstream"
		bool bKeepReading = true;
		do
		{
			infoFile >> a;		// Title_End??
			if ( a != "Title_End" )
			{
				ssTitle << a << " ";
			}
			else
			{	// it IS the end! 
				bKeepReading  = false;
				title = ssTitle.str();
			}
		}while( bKeepReading );


	}//if ( ! infoFile.is_open() )




    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


	glfwWindowHint(GLFW_DECORATED, 0);
	glfwWindowHint(GLFW_MAXIMIZED, 1);

	// C++ string
	// C no strings. Sorry. char    char name[7] = "Michael\0";
    ::g_pGLFWWindow = glfwCreateWindow( width, height, 
							            title.c_str(), 
							            NULL, NULL);
    if ( ! ::g_pGLFWWindow )
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }







    glfwSetKeyCallback( ::g_pGLFWWindow, key_callback );













	// For the FBO to resize when the window changes
	glfwSetWindowSizeCallback( ::g_pGLFWWindow, window_size_callback );

    glfwMakeContextCurrent( ::g_pGLFWWindow );
    gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress );
    glfwSwapInterval(1);



	std::cout << glGetString(GL_VENDOR) << " " 
		<< glGetString(GL_RENDERER) << ", " 
		<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// General error string, used for a number of items during start up
	std::string error;

	::g_pShaderManager = new cShaderManager();

	cShaderManager::cShader vertShader;
	cShaderManager::cShader fragShader;

	vertShader.fileName = "simpleVert.glsl";	
	//fragShader.fileName = "simpleFrag.glsl"; 
	fragShader.fileName = "simpleFragDeferred.glsl"; 

	::g_pShaderManager->setBasePath( "assets//shaders//" );







	// Shader objects are passed by reference so that
	//	we can look at the results if we wanted to. 
	::g_pShaderManager->createProgramFromFile("mySexyShader", vertShader, fragShader);








// Triangle debug renderer test...
	::g_pDebugRenderer = new cDebugRenderer();
	if ( ! ::g_pDebugRenderer->initialize(error) )
	{
		std::cout << "Warning: couldn't init the debug renderer." << std::endl;
		std::cout << "\t" << ::g_pDebugRenderer->getLastError() << std::endl;
	}

//	const float WORLDMAX = 25.0f;
//	::g_pDebugRenderer->addTriangle( glm::vec3( -WORLDMAX, 0.0f, 0.0f ),
//									 glm::vec3(WORLDMAX, 0.0f, 0.0f ),
//									 glm::vec3( 0.0f, WORLDMAX, 0.0f),
//									 glm::vec3( 1.0f, 1.0f, 1.0f ), 20.0f );
//	for (int count = 0; count != 100; count++)
//	{
//		::g_pDebugRenderer->addTriangle(
//			glm::vec3(getRandInRange(-WORLDMAX, WORLDMAX),
//			          getRandInRange(-WORLDMAX, WORLDMAX),
//			          getRandInRange(-WORLDMAX, WORLDMAX)),
//			glm::vec3(getRandInRange(-WORLDMAX, WORLDMAX), 
//			          getRandInRange(-WORLDMAX, WORLDMAX),
//			          getRandInRange(-WORLDMAX, WORLDMAX)),
//			glm::vec3(getRandInRange(-WORLDMAX, WORLDMAX),
//			          getRandInRange(-WORLDMAX, WORLDMAX),
//			          getRandInRange(-WORLDMAX, WORLDMAX)),
//			glm::vec3( 1.0f, 1.0f, 1.0f ), 15.0f );
//	}//for ...
//	::g_pDebugRenderer->addTriangle( glm::vec3( -50.0f, -25.0f, 0.0f ),
//									 glm::vec3( 0.0, 50.0f, 100.0f ),
//									 glm::vec3( 50.0f, -25.0, 0.0f),
//									 glm::vec3( 1.0f, 1.0f, 0.0f ), 1000.0f );





	// Load models
	::g_pModelAssetLoader = new cModelAssetLoader();
	::g_pModelAssetLoader->setBasePath("assets/models/");

	::g_pVAOManager = new cVAOMeshManager();

	GLint currentProgID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");


	GLint sexyShaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

	if ( ! Load3DModelsIntoMeshManager(sexyShaderID, ::g_pVAOManager, ::g_pModelAssetLoader, error ) )
	{
		std::cout << "Not all models were loaded..." << std::endl;
		std::cout << error << std::endl;
	}

	LoadModelsIntoScene();

	PhysicsHandler1->InitializePhysics(::PhysicsVector, ::gravity, ::collision, ::level);

	// Named unifrom block
	// Now many uniform blocks are there? 
	GLint numberOfUniformBlocks = -1;
	glGetProgramiv(currentProgID, GL_ACTIVE_UNIFORM_BLOCKS, &numberOfUniformBlocks);

	// https://www.opengl.org/wiki/GLAPI/glGetActiveUniformBlock

	// Set aside some buffers for the names of the blocks
	const int BUFFERSIZE = 1000;

	int name_length_written = 0;

	char NUBName_0[BUFFERSIZE] = {0};
	char NUBName_1[BUFFERSIZE] = {0};

	glGetActiveUniformBlockName(currentProgID,
								0,
								BUFFERSIZE,
								&name_length_written,
								NUBName_0);

	glGetActiveUniformBlockName(currentProgID,
								1,
								BUFFERSIZE,
								&name_length_written,
								NUBName_1);

//	NUB_lighting
//	NUB_perFrame

	GLuint NUB_Buffer_0_ID = 0;
	GLuint NUB_Buffer_1_ID = 0;

	glGenBuffers(1, &NUB_Buffer_0_ID);
	glBindBuffer(GL_UNIFORM_BUFFER, NUB_Buffer_0_ID );

	glGenBuffers(1, &NUB_Buffer_1_ID);
	glBindBuffer(GL_UNIFORM_BUFFER, NUB_Buffer_1_ID);




	// Get the uniform locations for this shader
//	mvp_location = glGetUniformLocation(currentProgID, "MVP");		// program, "MVP");


//	GLint uniLoc_diffuseColour = glGetUniformLocation( currentProgID, "diffuseColour" );

	::g_pLightManager = new cLightManager();

	::g_pLightManager->CreateLights(10);	// There are 10 lights in the shader

	::g_pLightManager->vecLights[0].setLightParamType(cLight::POINT);		
	::g_pLightManager->vecLights[0].attenuation.y = 0.000002f;		
	::g_pLightManager->vecLights[0].ambient = glm::vec3(0, 0, 1);

	::g_pLightManager->vecLights[1].position = glm::vec3(0.0f, 570.0f, 0.0f);	
	::g_pLightManager->vecLights[1].attenuation.y = 0.00456922280f;		//0.172113f;
	::g_pLightManager->vecLights[1].setLightParamType(cLight::SPOT);		// <--- DOH! This would explain why I couldn't light up the scene!!
	// Point spot straight down at the ground
	::g_pLightManager->vecLights[1].direction = glm::vec3(0.0f, -1.0f, 0.0f );
	::g_pLightManager->vecLights[1].setLightParamSpotPrenumAngleInner( glm::radians(15.0f) );
	::g_pLightManager->vecLights[1].setLightParamSpotPrenumAngleOuter( glm::radians(45.0f) );
	::g_pLightManager->vecLights[1].position = glm::vec3(0.0f, 50.0f, 0.0f);	
	//(-300, -100, 1000)
	::g_pLightManager->vecLights[2].attenuation.y = 0.00456922280f;		//0.172113f;
	::g_pLightManager->vecLights[2].setLightParamType(cLight::POINT);
	::g_pLightManager->vecLights[2].direction = glm::vec3(0.0f, -1.0f, 0.0f);
	::g_pLightManager->vecLights[2].setLightParamSpotPrenumAngleInner(glm::radians(15.0f));
	::g_pLightManager->vecLights[2].setLightParamSpotPrenumAngleOuter(glm::radians(45.0f));
	::g_pLightManager->vecLights[2].position = glm::vec3(-300, 200, 1000);



	glm::vec3 lavaLocation = glm::vec3(-800, -200, 100);
	::g_pLightManager->vecLights[4].attenuation.y = 0.00056922280f;		//0.172113f;
	::g_pLightManager->vecLights[4].setLightParamType(cLight::POINT);
	::g_pLightManager->vecLights[4].direction = glm::vec3(0.0f, -1.0f, 0.0f);
	::g_pLightManager->vecLights[4].position = lavaLocation + glm::vec3(0, 200, 0);
	::g_pLightManager->LoadShaderUniformLocations(currentProgID);


	//glm::vec3(-1500, -100, -200)
	
	// Texture 
	::g_pTextureManager = new CTextureManager();

	std::cout << "GL_MAX_TEXTURE_IMAGE_UNITS: " << ::g_pTextureManager->getOpenGL_GL_MAX_TEXTURE_IMAGE_UNITS() << std::endl;
	std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: " << ::g_pTextureManager->getOpenGL_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS() << std::endl;





	//TEXTURES GET LOADED HERE:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	::g_pTextureManager->setBasePath("assets/textures");
	//::g_pTextureManager->Create2DTextureFromBMPFile("plane_normal_map.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("Nimage.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("Nmap.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("lava1.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("lava1_hm.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("lava2.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("lava2_hm.bmp", true);
	//::g_pTextureManager->Create2DTextureFromBMPFile("Iron_Plate/Iron_Plate_SPC.bmp", true);
	//::g_pTextureManager->Create2DTextureFromBMPFile("Iron_Plate/Iron_Plate_SPC.bmp", true);


	::g_pTextureManager->setBasePath("assets/textures/skybox");
	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
			"space",
			"nebula_front.bmp",
			"nebula_bk.bmp",
			"nebula_up.bmp",
			"nebula_down.bmp",
			"nebula_right.bmp",
			"nebula_left.bmp", true, true))
	{
		std::cout << "Didn't load skybox" << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
///***********************************************************
	// About the generate the AABB for the terrain
	::g_terrainAABBBroadPhase = new cAABBBroadPhase();
	// Perhaps you'd like something more sophisticated than this...
	::g_terrainAABBBroadPhase->pDebugRenderer = ::g_pDebugRenderer;
	// 
	cMesh terrainMesh; 
	if (::g_pVAOManager->lookupMeshFromName("MeshLabTerrain_xyz_n_uv", terrainMesh))
	{
		std::cout << "Generating the terrain AABB grid. This will take a moment..." << std::endl;
		
		::g_terrainAABBBroadPhase->genAABBGridFromMesh(terrainMesh);

	}//if (::g_pVAOManager->lookupMeshFromName
///***********************************************************


	::g_pTheCamera = new cCamera();
	::g_pTheCamera->setCameraMode(cCamera::MODE_FLY_USING_LOOK_AT);
	::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(0.0f, 10.0f, 650.0f));
	::g_pTheCamera->FlyCamLA->setTargetInWorld(glm::vec3(0.0f, 20.0f, 0.0f));
	::g_pTheCamera->FlyCamLA->setUpVector(glm::vec3(0.0f, 1.0f, 0.0f));


	::g_pTheCamera2 = new cCamera();
	::g_pTheCamera2->setCameraMode(cCamera::MODE_FLY_USING_LOOK_AT);
	::g_pTheCamera2->FlyCamLA->setEyePosition(glm::vec3(0.0f, 200.0f, 300.0f));
	::g_pTheCamera2->FlyCamLA->setTargetInWorld(glm::vec3(0.0f, 0.0f, 0.0f));
	::g_pTheCamera2->FlyCamLA->setUpVector(glm::vec3(0.0f, 1.0f, 0.0f));

	::g_pPhysicsWorld = new cPhysicsWorld();


	// All loaded!
	std::cout << "And we're good to go! Staring the main loop..." << std::endl;

	glEnable( GL_DEPTH );


	// Create an FBO
	if ( ! ::pass1_cFBO.init(1920, 1080, error) )
	{
		std::cout << "::pass2_cFBO error: " << error << std::endl;
	}
	else
	{
		std::cout << "::pass2_cFBO is good." << std::endl;
		std::cout << "\t::pass1_cFBO ID = " << ::pass1_cFBO.ID << std::endl;
		std::cout << "\tcolour texture ID = " << ::pass1_cFBO.colourTexture_0_ID << std::endl;
		std::cout << "\tnormal texture ID = " << ::pass1_cFBO.normalTexture_1_ID << std::endl;

		std::cout << "GL_MAX_COLOR_ATTACHMENTS = " << ::pass1_cFBO.getMaxColourAttachments() << std::endl;
		std::cout << "GL_MAX_DRAW_BUFFERS = " << ::pass1_cFBO.getMaxDrawBuffers() << std::endl;

	}//if ( ! ::pass1_cFBO.init

	if ( ! ::pass2_cFBO.init(1920, 1080, error) )
	{
		std::cout << "::pass2_cFBO error: " << error << std::endl;
	}
	else
	{
		std::cout << "FBO is good." << std::endl;
		std::cout << "\t::pass2_cFBO ID = " << ::pass2_cFBO.ID << std::endl;
		std::cout << "\tcolour texture ID = " << ::pass2_cFBO.colourTexture_0_ID << std::endl;
		std::cout << "\tnormal texture ID = " << ::pass2_cFBO.normalTexture_1_ID << std::endl;

		std::cout << "GL_MAX_COLOR_ATTACHMENTS = " << ::pass2_cFBO.getMaxColourAttachments() << std::endl;
		std::cout << "GL_MAX_DRAW_BUFFERS = " << ::pass2_cFBO.getMaxDrawBuffers() << std::endl;

	}//if ( ! ::pass2_cFBO.init




	setWindowFullScreenOrWindowed( ::g_pGLFWWindow, ::g_IsWindowFullScreen );


	// Gets the "current" time "tick" or "step"
	double lastTimeStep = glfwGetTime();






	//// Spawn the tread to update the Dalek 
	//unsigned int DalekThreadHandle = 0;
	//unsigned threadID = 0;

	//// Creates critical section, etc.
	//InitDalekThreading();

	//DalekThreadHandle = _beginthreadex(
	//	NULL,			// Don't change the security permissions for this thread
	//	0,				// 0 = default stack
	//	DalekBrainThread,		// The function we want to call. 
	//	NULL, //pDataToPassA,			// Arguement list (or NULL if we are passing VOID)
	//	0,				// or CREATE_SUSPENDED if it's paused and has to start
	//	&threadID);


	// Allow the Daleks to update
	
	
	
	
	//DALEKS
	/////////////////////////////////////////////////////////////////////////////////
	
	//::g_pDalekManager->SetIsUpdatingOnAllDaleks(true);

	// Could also:
	// 1. Start the threads as CREATE_SUSPENDED
	// 2. Then go through all the threads and call 
	//    ResumeThread() on each one. 
	// Never call SuspendThead
	/////////////////////////////////////////////////////////////////////////////////

	bool bHACK_RUN_ONCE_TO_SET_LIGHT = false;



	EngageForces();






	glm::vec3 SmoothCameraAccumulator = glm::vec3(0, 1, 0);

	setUpTheCamera();
	::g_pTheCamera->FlyCamLA->setTargetInWorld(::g_vecCollisionObjects[0]->getPosition());






	cGameObject* pTempGO = new cGameObject();
	cPhysicalProperties physState;
	physState.position = glm::vec3(0, 200, 400);
	physState.rigidBodyShape = physState.SPHERE;
	physState.rigidBodyMeasurements.sphere_capsule_radius = 10.0f;
	pTempGO->SetPhysState(physState);
	sMeshDrawInfo meshInfo;
	meshInfo.bIsVisible = 0;
	meshInfo.debugDiffuseColour = glm::vec4(0.0f,0.0f,0.0f,0.0f);
	meshInfo.bDrawAsWireFrame = 1;
	meshInfo.scale = 4.0f;
	meshInfo.name = "technosphere";
	meshInfo.bIsEnvirMapped = false;
	pTempGO->friendlyName = "raycastpoint";
	pTempGO->vecMeshes.push_back(meshInfo);

	::g_vecVisualObjects.push_back(pTempGO);		// Fastest way to add






	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.position = glm::vec3(0, 200, 400);
		//physState.setOrientationQ(glm::vec3(90, 0, 0));
		pTempGO->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.bUseDebugColour = false;
		meshInfo.scale = 2.0f;
		meshInfo.name = "screen";
		meshInfo.bIsEnvirMapped = false;
		meshInfo.reflectBlendRatio = 0.0f;
		meshInfo.refractBlendRatio = 0.0f;


		meshInfo.coefficientRefract = 0.0f;
		pTempGO->friendlyName = "portal";
		meshInfo.bDisableBackFaceCulling = 1;
		//meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("Iron_Plate/Iron_Plate_SPC.bmp", 1.0f));
		pTempGO->vecMeshes.push_back(meshInfo);
		myInterface::PhysicsObject tempobj;
		tempobj.ApplyPhysics = 1.0f;
		tempobj.Mass = 0;
		tempobj.ObjectName = "portal";
		tempobj.PhysicsPosition = physState.position;
		tempobj.PhysicsScale = meshInfo.scale;
		tempobj.type = myInterface::SPHERE;
		::PhysicsVector.push_back(tempobj);

		::g_vecVisualObjects.push_back(pTempGO);		// Fastest way to add

	}


	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.position = glm::vec3(0, 200, 950);
		physState.adjOrientationEulerAngles(glm::vec3(0, 90, 0), true);
		//physState.setOrientationQ(glm::vec3(90, 0, 0));
		pTempGO->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.bUseDebugColour = false;
		meshInfo.scale = 2.0f;
		meshInfo.name = "screen";
		meshInfo.bIsEnvirMapped = false;
		meshInfo.reflectBlendRatio = 0.0f;
		meshInfo.refractBlendRatio = 0.0f;


		meshInfo.coefficientRefract = 0.0f;
		pTempGO->friendlyName = "portal";
		meshInfo.bDisableBackFaceCulling = 1;
		//meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("Iron_Plate/Iron_Plate_SPC.bmp", 1.0f));
		pTempGO->vecMeshes.push_back(meshInfo);
		myInterface::PhysicsObject tempobj;
		tempobj.ApplyPhysics = 1.0f;
		tempobj.Mass = 0;
		tempobj.ObjectName = "portal";
		tempobj.PhysicsPosition = physState.position;
		tempobj.PhysicsScale = meshInfo.scale;
		tempobj.type = myInterface::SPHERE;
		::PhysicsVector.push_back(tempobj);

		::g_vecVisualObjects.push_back(pTempGO);		// Fastest way to add

	}


	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.position = lavaLocation;
		physState.adjOrientationEulerAngles(glm::vec3(0, 0, 0), true);
		//physState.setOrientationQ(glm::vec3(90, 0, 0));
		pTempGO->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.bUseDebugColour = false;
		meshInfo.scale = 20.0f;
		meshInfo.name = "plane";
		meshInfo.bIsEnvirMapped = false;
		meshInfo.reflectBlendRatio = 0.0f;
		meshInfo.refractBlendRatio = 0.0f;


		meshInfo.coefficientRefract = 0.0f;
		pTempGO->friendlyName = "plane";
		meshInfo.bDisableBackFaceCulling = 1;
		meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("lava1.bmp", 1.0f));
		meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("lava2.bmp", 0.0f));
		pTempGO->vecMeshes.push_back(meshInfo);
		myInterface::PhysicsObject tempobj;
		tempobj.ApplyPhysics = 1.0f;
		tempobj.Mass = 0;
		tempobj.ObjectName = "plane";
		tempobj.PhysicsPosition = physState.position;
		tempobj.PhysicsScale = meshInfo.scale;
		tempobj.type = myInterface::PLANE;
		::PhysicsVector.push_back(tempobj);

		::g_vecVisualObjects.push_back(pTempGO);		// Fastest way to add

	}

	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.position = glm::vec3(0, 200, 400);
		physState.rigidBodyShape = physState.SPHERE;
		physState.rigidBodyMeasurements.sphere_capsule_radius = 2.0f;
		pTempGO->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.debugDiffuseColour = glm::vec4(1, 1, 1, 1);
		meshInfo.bDrawAsWireFrame = 1;
		meshInfo.scale = 0.2f;
		meshInfo.name = "technosphere";
		meshInfo.bIsEnvirMapped = false;
		pTempGO->friendlyName = "raycastpoint";
		pTempGO->vecMeshes.push_back(meshInfo);

		::g_vecVisualObjects.push_back(pTempGO);		// Fastest way to add

	}


	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.position = glm::vec3(-300, -100, 1000);
		physState.adjOrientationEulerAngles(glm::vec3(0, 0, 0), true);
		//physState.setOrientationQ(glm::vec3(90, 0, 0));
		pTempGO->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.bUseDebugColour = false;
		meshInfo.scale = 20.0f;
		meshInfo.bDisableDepthTest = 0;
		meshInfo.name = "Mountain";
		meshInfo.bIsEnvirMapped = false;
		meshInfo.reflectBlendRatio = 0.0f;
		meshInfo.refractBlendRatio = 0.0f;


		meshInfo.coefficientRefract = 0.0f;
		pTempGO->friendlyName = "Mountain1";
		meshInfo.bDisableBackFaceCulling = 1;
		//meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("Nimage.bmp", 1.0f));
		//meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("Nmap.bmp", 0.0f));
		pTempGO->vecMeshes.push_back(meshInfo);
		myInterface::PhysicsObject tempobj;
		tempobj.ApplyPhysics = 1.0f;
		tempobj.Mass = 0;
		tempobj.ObjectName = "Mountain";
		tempobj.PhysicsPosition = physState.position;
		tempobj.PhysicsScale = meshInfo.scale;
		tempobj.type = myInterface::PLANE;
		//::PhysicsVector.push_back(tempobj);

		::g_vecVisualObjects.push_back(pTempGO);		// Fastest way to add

	}





	ray = g_vecCollisionObjects[0]->getPosition();

	glm::vec3 ray2 = g_vecCollisionObjects[0]->getPosition();


	// Main game or application loop
	while (!glfwWindowShouldClose(::g_pGLFWWindow))
	{




		// Essentially the "frame time"
		// Now many seconds that have elapsed since we last checked
		double curTime = glfwGetTime();
		::deltaTime = curTime - lastTimeStep;
		lastTimeStep = curTime;


		::g_pLightManager->vecLights[0].position = g_vecCollisionObjects[0]->getPosition();


		if (::changelevel)
		{
			LoadGameLevel(::level, ::changelevel);
			PhysicsHandler1->InitializePhysics(::PhysicsVector, ::gravity, ::collision, ::level);
			//::g_vecCollisionObjects[2]->bIsPortal = 1;
			::g_vecVisualObjects[1]->bIsPortal = 1;
			::g_vecVisualObjects[2]->bIsPortal = 1;

			::g_vecCollisionObjects[0]->bIsBall = 1;

		}


		if (glm::distance(::g_vecVisualObjects[1]->getPosition(), ::g_vecCollisionObjects[0]->getPosition()) <= 150.0f)
		{
			::respawn = glm::vec4(1, 0, 200, 900);
			::oldup = ::up;
			::up = glm::round(glm::vec3(0, 1, 0));
			::up_change = 1;
		}

		//	std::cout << glm::distance(::g_vecVisualObjects[1]->getPosition(), ::g_vecCollisionObjects[0]->getPosition())<< std::endl;





		::gravity = (float)-200 * ::up;
		UpdateForces();
		SmoothKeys((float)deltaTime);
		PhysicsHandler1->PhysicsStep(::deltaTime, ForceVector, ::PhysicsVector, ::gravity, 0, collision, respawn, ::level);
		if (jumptime + 1.0 >= glfwGetTime())
		{
			::SPACE = 0;
			jumplock = 1;
			//::jumptime = glfwGetTime();
		}
		else
		{
			jumplock = 0;
		}


		for (int i = 0; i < g_vecCollisionObjects.size(); i++)
		{
			g_vecCollisionObjects[i]->overwritePotition(::PhysicsVector[i].PhysicsPosition);
			g_vecCollisionObjects[i]->overwriteQOrientation(::PhysicsVector[i].PhysicsOrientation2);

		}

		// CAMERA
		::g_pTheCamera->FlyCamLA->setTargetInWorld(::g_vecCollisionObjects[0]->getPosition());
		if (::up.x == 1)
		{
			::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x + 3.5*CAMERAFACTOR, ::g_vecCollisionObjects[0]->getPosition().y + (CAMERAFACTOR + vertDistance) * cos(theta), ::g_vecCollisionObjects[0]->getPosition().z + (CAMERAFACTOR + vertDistance) * sin(theta)));

		}
		else if (::up.y == 1)
		{
			::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x + (CAMERAFACTOR + vertDistance) * cosf(theta), ::g_vecCollisionObjects[0]->getPosition().y + 3.5*CAMERAFACTOR, ::g_vecCollisionObjects[0]->getPosition().z + (CAMERAFACTOR + vertDistance) * sin(theta)));

		}
		else if (::up.z == 1)
		{
			::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x + (CAMERAFACTOR + vertDistance) * cosf(theta), ::g_vecCollisionObjects[0]->getPosition().y + (CAMERAFACTOR + vertDistance) * sin(theta), ::g_vecCollisionObjects[0]->getPosition().z + 3.5*CAMERAFACTOR));

		}
		else if (::up.x == -1)
		{
			::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x - 1.5*CAMERAFACTOR, ::g_vecCollisionObjects[0]->getPosition().y + (CAMERAFACTOR + vertDistance) * sin(theta), ::g_vecCollisionObjects[0]->getPosition().z + (CAMERAFACTOR + vertDistance) * cos(theta)));

		}
		else if (::up.y == -1)
		{
			::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x + (CAMERAFACTOR + vertDistance) * sin(theta), ::g_vecCollisionObjects[0]->getPosition().y - 1.5*CAMERAFACTOR, ::g_vecCollisionObjects[0]->getPosition().z + (CAMERAFACTOR + vertDistance) * cos(theta)));

		}
		else if (::up.z == -1)
		{
			::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(::g_vecCollisionObjects[0]->getPosition().x + (CAMERAFACTOR + vertDistance) * sin(theta), ::g_vecCollisionObjects[0]->getPosition().y + (CAMERAFACTOR + vertDistance) * cos(theta), ::g_vecCollisionObjects[0]->getPosition().z - 1.5*CAMERAFACTOR));

		}
		else
		{

		}
		if (gravitychange != ::up)
		{
			Audio->OutputResult = Audio->Sounds[1]->setMode(FMOD_LOOP_OFF);
			Audio->OutputResult = Audio->SoundSystem->playSound(Audio->Sounds[1], 0, false, &Audio->ChannelList[1]);
			gravitychange = ::up;
		}



		if (::up_change == 1)
		{
			SmoothCameraAccumulator = ::oldup;
			::up_change = 0;
		}

		if (glm::abs(SmoothCameraAccumulator - ::up).x <= 0.1 && glm::abs(SmoothCameraAccumulator - ::up).y <= 0.1 && glm::abs(SmoothCameraAccumulator - ::up).z <= 0.1)
		{
			::up_change = 0;
		}
		else
		{
			SmoothCameraAccumulator += (::up - ::oldup)*(float)deltaTime;
			::g_pTheCamera->FlyCamLA->setUpVector(SmoothCameraAccumulator);
		}
		//////////////





		exitwhile = 0;
		int thisIsMirror;
		bool mirrorConfirmed;
		ray += glm::normalize(::g_vecCollisionObjects[0]->getPosition() + ::up*5.0f - ::g_pTheCamera->getEyePosition() + ::up*20.0f)*100.0f;
		//::g_vecVisualObjects[0]->vecMeshes[0].debugDiffuseColour = glm::vec4(0,0,0,0);


		for (int i = 1; i < ::g_vecCollisionObjects.size() && exitwhile == 0; i++)
		{
			if (glm::distance(ray, ::g_vecCollisionObjects[i]->getPosition()) <= 150 && glm::distance(::g_vecCollisionObjects[0]->getPosition(), ::g_vecCollisionObjects[i]->getPosition()) >= 200)
			{
				//	::g_vecVisualObjects[0]->vecMeshes[0].debugDiffuseColour = glm::vec4(0, 0, 0, 0);
				ray = ::g_vecCollisionObjects[0]->getPosition() + glm::vec3(0, 15, 0);
				thisIsMirror = i;
				mirrorConfirmed = 1;
				exitwhile = 1;

			}

		}
		if (glm::distance(::g_vecCollisionObjects[0]->getPosition(), ray) >= 2000)
		{
			ray = ::g_vecCollisionObjects[0]->getPosition() + glm::vec3(0, 5, 0);
			exitwhile = 1;
		}

		::g_vecVisualObjects[0]->overwritePotition(ray + glm::vec3(0, 5, 0));





		for (int i = 1; i < ::g_vecCollisionObjects.size(); i++)
		{
			if (mirrorConfirmed)
			{
				if (i == thisIsMirror)
				{
					::g_vecCollisionObjects[i]->bIsPortal = true;
				}
				else
				{
					::g_vecCollisionObjects[i]->bIsPortal = false;

				}
			}
		}





		::g_vecVisualObjects[4]->overwritePotition(::PhysicsVector[0].PhysicsPosition + ::up*18.0f);

		////////////////////////////////////////////////////////////////////////////////////////////////////


		//::ParticleSystem1->genLocation = ::g_vecCollisionObjects[0]->getPosition();
		::ParticleSystem1->genLocation = glm::vec3(0, 50, 200);
		//::ParticleSystem1->EnableSmokePositionBasedMotion();
		::ParticleSystem1->GeneratorType = ::ParticleSystem::FIRE;


		if (::particleEnable)
		{
			::ParticleSystem1->genLocation = lavaLocation + glm::vec3(0, -100, 0);
			::ParticleSystem1->wind = ::windDirection;
			::ParticleSystem1->createParticle(200, 1, 0.5, 4);
			::ParticleSystem1->updateParticles(0.4);
			::ParticleSystem1->deleteOldParticles(4);
		}
		else
		{
			::g_vecParticleObjects.clear();
		}
		
	
		//draw_insance
			
			

		//exitwhile = 0;
		//int thisIsMirror;
		//bool mirrorConfirmed;
		//ray += glm::normalize(::g_vecCollisionObjects[0]->getPosition() + ::up*5.0f - ::g_pTheCamera->getEyePosition() + ::up*20.0f)*100.0f;
		////::g_vecVisualObjects[0]->vecMeshes[0].debugDiffuseColour = glm::vec4(0,0,0,0);


		//for (int i = 1; i < ::g_vecCollisionObjects.size() && exitwhile == 0; i++)
		//{
		//	if (glm::distance(ray, ::g_vecCollisionObjects[i]->getPosition()) <= 150 && glm::distance(::g_vecCollisionObjects[0]->getPosition(), ::g_vecCollisionObjects[i]->getPosition()) >= 200)
		//	{
		//		//	::g_vecVisualObjects[0]->vecMeshes[0].debugDiffuseColour = glm::vec4(0, 0, 0, 0);
		//		ray = ::g_vecCollisionObjects[0]->getPosition() + glm::vec3(0, 15, 0);
		//		thisIsMirror = i;
		//		mirrorConfirmed = 1;
		//		exitwhile = 1;

		//	}

		//}
		//if (glm::distance(::g_vecCollisionObjects[0]->getPosition(), ray) >= 2000)
		//{
		//	ray = ::g_vecCollisionObjects[0]->getPosition() + glm::vec3(0, 5, 0);
		//	exitwhile = 1;
		//}
		if (::level == 1)
		{
			bool stopSearching = 0;
			int Enemy = 0;
			bool bFoundEnemy = 0;

			if (::shoot || ::tag)
			{
				if (::shoot)
				{
					Audio->OutputResult = Audio->Sounds[1]->setMode(FMOD_LOOP_OFF);

				}
				if (::tag)
				{
					Audio->OutputResult = Audio->Sounds[1]->setMode(FMOD_LOOP_OFF);
				}

				::g_vecTaggingObjects[0]->vecMeshes[0].bIsVisible = 1;

				ray2 += glm::normalize(::PhysicsVector[0].PhysicsPosition + ::up*18.0f - ::g_pTheCamera->getEyePosition())*(float)deltaTime*2500.0f;
				for (int i = 1; i < ::g_vecTaggingObjects.size() && !stopSearching; i++)
				{
					if (glm::distance(ray2, ::g_vecTaggingObjects[i]->getPosition()) <= 35.0f)
					{
						//	::g_vecVisualObjects[0]->vecMeshes[0].debugDiffuseColour = glm::vec4(0, 0, 0, 0);
						ray2 = ::g_vecCollisionObjects[0]->getPosition() + glm::vec3(0, 5, 0);
						Enemy = i;
						bFoundEnemy = 1;
						stopSearching = 1;
					}
				}
				if (glm::distance(::g_vecCollisionObjects[0]->getPosition(), ray2) >= 2000)
				{
					ray2 = ::g_vecCollisionObjects[0]->getPosition() + glm::vec3(0, 5, 0);
					shoot = 0;
					tag = 0;
				}
			}
			else
			{
				::g_vecTaggingObjects[0]->vecMeshes[0].bIsVisible = 0;
				ray2 = ::PhysicsVector[0].PhysicsPosition;
			}
		
			if (bFoundEnemy)
			{
				
				if (::tag)
				{
					for (int i = 0; i<::g_vecTaggingObjects.size(); i++)
					{
						::g_vecTaggingObjects[i]->vecMeshes[0].bDisableDepthTest = 0;
						::g_vecTaggingObjects[i]->drawred = 0;
					}
					::g_vecTaggingObjects[Enemy]->vecMeshes[0].bDisableDepthTest = 1;
					::g_vecTaggingObjects[Enemy]->drawred = 1;
					::tag = 0;
				}
				if (::shoot)
				{
					::g_vecTaggingObjects.erase(::g_vecTaggingObjects.begin() + Enemy);
					::shoot = 0;
				}

				bFoundEnemy = 0;
			}
			::g_vecTaggingObjects[0]->overwritePotition(ray2);
		}
		
		






























	//	setUpTheCamera();


		// Call the "thread" function
		//::g_DeltaTime = ::deltaTime;
//		::g_SetDeltaTime(::deltaTime);
//		DalekBrainThread(NULL);
//
//		// Copy the Dalek information to the actual game object
//		cGameObject* pDalekGO = findObjectByFriendlyName("Big D", ::g_vecGameObjects);
//		cPhysProps state;
//		pDalekGO->GetPhysState(state);
////		state.position = ::g_DalekPosition;
//		state.position = ::g_GetDalekPosition();
//		pDalekGO->SetPhysState( state );
//		std::cout 
//			<< state.position.x << ", " 
//			<< state.position.y << ", " 
//			<< state.position.z << std::endl;
//
		// ************************************************************
//		mapDalekID_to_GameObject;
//			virtual bool getDalekPositionsAtIndexRange( unsigned int startIndex, unsigned int endIndex, 
//									             );



		//DALEKS
		/////////////////////////////////////////////////////////////////////////////////



		//::g_pDalekManager->getAllDalekPositions( vecDalekPositions );
		//for ( unsigned int index = 0; index != vecDalekPositions.size(); index++ )
		//{
		//	cPhysProps DalekPhysState;
		//	cGameObject* pDalekGO = mapDalekID_to_GameObject[index];
		//	pDalekGO->GetPhysState( DalekPhysState );
		//	DalekPhysState.position = vecDalekPositions[index];
		//	pDalekGO->SetPhysState( DalekPhysState );
		//}

		//// For lockless, swich buffers
		//::g_pDalekManager->SwitchBuffers();



		/////////////////////////////////////////////////////////////////////////////////

		//meshInfo.vecMehs2DTextures.
		
		//::blendvar1 += (float)blendvar1Sign*(float)::deltaTime / 50.0f;
		
		if (::blendvar1 < 0)
		{
			posBlend = abs(::blendvar1);
		}
		else
		{
			posBlend = ::blendvar1;
		}

		::g_vecVisualObjects[3]->vecMeshes[0].vecMehs2DTextures[0].blendRatio = posBlend;
		::g_vecVisualObjects[3]->vecMeshes[0].vecMehs2DTextures[1].blendRatio = 1 - posBlend;


		// ************************************************************



		::g_pPhysicsWorld->IntegrationStep(::deltaTime);

		::g_pTheCamera->updateTick(::deltaTime);
		::g_pTheCamera2->updateTick(::deltaTime);

		// *********************************************
		//    ___ _        ___              __     ___                          
		//   / __| |___  _| _ ) _____ __  __\ \   / __|__ _ _ __  ___ _ _ __ _  
		//   \__ \ / / || | _ \/ _ \ \ / |___> > | (__/ _` | '  \/ -_) '_/ _` | 
		//   |___/_\_\\_, |___/\___/_\_\    /_/   \___\__,_|_|_|_\___|_| \__,_| 
		//            |__/                                                      
		cPhysicalProperties skyBoxPP;
		::g_pSkyBoxObject->GetPhysState(skyBoxPP);
		skyBoxPP.position = ::g_pTheCamera->getEyePosition();
		::g_pSkyBoxObject->SetPhysState(skyBoxPP);



//    ___                _             _           ___       _            __   __           
//   | _ \ ___  _ _   __| | ___  _ _  | |_  ___   / __| ___ | |__  _  _  / _| / _| ___  _ _ 
//   |   // -_)| ' \ / _` |/ -_)| '_| |  _|/ _ \ | (_ ||___|| '_ \| || ||  _||  _|/ -_)| '_|
//   |_|_\\___||_||_|\__,_|\___||_|    \__|\___/  \___|     |_.__/ \_,_||_|  |_|  \___||_|  
//                                                                        
// In this pass, we render all the geometry to the "G buffer"
// The lighting is NOT done here. 
// 
		glm::vec3 mycamloc;
		glm::vec3 tarset;


		pass1_cFBO.clearBuffers();

		//tarset = ::g_vecCollisionObjects[0]->position;
		mycamloc = ::g_pTheCamera->getEyePosition();
		::g_pShaderManager->useShaderProgram("mySexyShader");
		GLint shaderPassID = glGetUniformLocation(sexyShaderID, "prakash");
		glUniform1i( shaderPassID, PassInt0 );
		glBindFramebuffer(GL_FRAMEBUFFER, pass1_cFBO.ID );
		
		
		//pass1_cFBO.clearBuffers();

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Clear colour is BLACK

		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_STENCIL_TEST);
		RenderScene(::g_vecGameObjects, ::g_pGLFWWindow, ::deltaTime);
		RenderScene(::g_vecCollisionObjects, ::g_pGLFWWindow, ::deltaTime);
		RenderScene(::g_vecParticleObjects, ::g_pGLFWWindow, ::deltaTime);
		RenderScene(::g_vecVisualObjects, ::g_pGLFWWindow, ::deltaTime);
		RenderScene(::g_vecTaggingObjects, ::g_pGLFWWindow, ::deltaTime);

		//glDisable(GL_DEPTH_TEST);
//glDisable(GL_DEPTH_TEST);		// Check what's already in the depth buffer
//glDepthMask(GL_FALSE);			// Written to the depth buffer

		// 1. Drawing the "mask" object (that "stencil")

		// Note the addition to the stencil buffer clear
		// BIG NOTE: This clears the FINAL frame buffer! 
		
		//****************************
		//glDrawElementsInstanced()

		// (Keep in mind that when the stencil is enabled, it's enabled for ALL frame buffers)
		
		

		//NOTHING HAPPENS WHEN COMMENTED OUT
		//*****************************************************************************
		//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		//glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		//*****************************************************************************

		




		////////////////
//
//		glEnable(GL_STENCIL_TEST);
//		glStencilFunc(GL_ALWAYS, 1, 0xFF);		// All 1s: 1111 1111
//		glStencilFunc(GL_GEQUAL, 1, 0xFF);		// All 1s: 1111 1111
//		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//		glStencilMask(0xFF);		// Control of writing to the buffer
//		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
////	glDisable(GL_DEPTH_TEST);
////	glDepthMask(GL_FALSE);
//
//		std::vector< cGameObject* > vecOnlyTheRoomMask;
//		vecOnlyTheRoomMask.push_back( ::g_RoomMaskForStencil);
//		RenderScene(vecOnlyTheRoomMask, ::g_pGLFWWindow, ::deltaTime );
//
//  	glClear(GL_COLOR_BUFFER_BIT );
//  	glClear(GL_DEPTH_BUFFER_BIT );

//		glStencilFunc(GL_ALWAYS, 0, 0xFF);
//		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//		// Draw ONLY the room
//		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//  	glEnable(GL_DEPTH_TEST);
//	    glDepthMask(GL_TRUE);
//		std::vector< cGameObject* > vecOnlyTheRoom;
//		vecOnlyTheRoom.push_back( ::g_Room );
//		RenderScene( vecOnlyTheRoom, ::g_pGLFWWindow, ::deltaTime );


//////////////////////////////////////////////////////////////////////////

//DRAWS THE REST OF THE SCENE
		
		


// Where it's 1, draw the scene 
// - remember, wherever we drew the masking object, it's 1
// - the rest of the scene is sill zero 
// So this passes where every the 0 is less than the stencil buffer
// - The stencil has 1 where the mask is, and zero everywhere else

		
		//glStencilFunc(GL_EQUAL, 1, 0xFF);
//glStencilMask(0x00);		// Make sure we can't write to the stencil buffer
// BUT, we DON'T want to change what's already on the stencil buffer
//glStencilOp(GL_KEEP,		// Stencil fail
					//GL_KEEP,		// Depth fail
					//GL_KEEP);		// Stencil AND Depth PASS







	

		
		
		
		// Also set the texture in the VERTEX shader...


		
		
		
		
		// At this pointer EVERYTHING in the scene is rendered to the "G-Buffer"


//// *********************************************************************************
//		// NOTE: The RenderScene eventually updates the skinned mesh info, 
//		//			which is now stored into the skinned mesh information
//		// When I call this, if it's a skinned mesh, the extents will be calculated, too
//		// (like for that pose on this frame)
//		// Find one of the skinned meshes
//		cGameObject* pSophieT = findObjectByFriendlyName( "Sophie", ::g_vecGameObjects );
//
//		// This box is "object relative" (i.e. around origin of the model, not in the world)
//		glm::vec3 minXYZ = pSophieT->vecMeshes[0].minXYZ_from_SM_Bones;
//		glm::vec3 maxXYZ = pSophieT->vecMeshes[0].maxXYZ_from_SM_Bones;
//
////	// Transform this based on where the character is in the world...
//	// BUT HOW, you? If only there was a matrix that described where the object was!!!
//	{
//		cPhysicalProperties sophPhysState;
//		pSophieT->GetPhysState(sophPhysState);
//
//		glm::mat4 matSophieWorld = glm::mat4(1.0f);		// identity
//		glm::mat4 trans = glm::mat4x4(1.0f);
//		matSophieWorld = glm::translate(matSophieWorld, sophPhysState.position );
//		glm::mat4 postRotQuat = glm::mat4(pSophieT->getFinalMeshQOrientation(sophPhysState.get));
//		matSophieWorld = matSophieWorld * postRotQuat;
//		glm::mat4 matScale = glm::mat4x4(1.0f);
//		matScale = glm::scale(matScale,
//							  glm::vec3(pSophieT->GetPhysState,
//							  theMesh.scale,
//							  theMesh.scale));
//		matSophieWorld = matSophieWorld * matScale;
//	}
//
//		//pSophieT->vecMeshes[0].vecObjectBoneTransformation[boneIndex]
//
//		g_pDebugRenderer->addTriangle( minXYZ, maxXYZ, minXYZ,
//									   glm::vec3(0.0,0.0,0.0) );
//		g_pDebugRenderer->addTriangle( maxXYZ, minXYZ,maxXYZ, 
//									   glm::vec3(0.0,0.0,0.0) );
//
//		// Draw a triangle at each bone... 
//		// Note we have to translate the location for each debug triangle.
//		// (but the "boneLocationXYZ" is the centre of the bone)
////		for (unsigned int boneIndex = 0; 
////			 boneIndex != pSophieT->vecMeshes[0].vecObjectBoneTransformation.size();
////			 boneIndex++)
////		{
////			glm::mat4 boneLocal = pSophieT->vecMeshes[0].vecObjectBoneTransformation[boneIndex];
////	
////			float scale = pSophieT->vecMeshes[0].scale;
////			boneLocal = glm::scale(boneLocal, glm::vec3(scale, scale, scale));
////	
////	
////			//glm::vec4 boneBallLocation = boneLocal * GameObjectLocalOriginLocation;
////			glm::vec4 boneLocationXYZ = boneLocal * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
////			boneLocationXYZ *= scale;
////	
////			// Draw a triangle at each bone...
////			glm::vec3 v[3];
////			v[0].x = boneLocationXYZ.x;
////			v[0].y = boneLocationXYZ.y;
////			v[0].z = boneLocationXYZ.z;
////			v[1] = v[0] + glm::vec3(2.0f, 0.0f, 0.0f);
////			v[2] = v[0] - glm::vec3(2.0f, 0.0f, 0.0f);
////	
////			g_pDebugRenderer->addTriangle( v[0], v[1], v[2], glm::vec3(1.0f, 1.0f, 1.0f) ); 
////		}
//
//		int indexOfFingerBone = pSophieT->pSimpleSkinnedMesh->m_mapBoneNameToBoneIndex["B_L_Finger02"];// = 32
//
//		// Transform to get the location in world space... 
//
//		// This vector of bones has the final location of that bone (this frame)
//		glm::mat4 boneLocal = pSophieT->vecMeshes[0].vecObjectBoneTransformation[indexOfFingerBone];
//
//		float scale = pSophieT->vecMeshes[0].scale;
//		boneLocal = glm::scale(boneLocal, glm::vec3(scale, scale, scale));
//
//		glm::vec4 B_L_Finger02_XYZ = boneLocal * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//		B_L_Finger02_XYZ *= scale;
//		// And transform based on object world...
//
//		// matWorld (or matObject) of the object
//
//		glm::vec3 fingerXYZ = glm::vec3(B_L_Finger02_XYZ);
//
//		g_pDebugRenderer->addTriangle( fingerXYZ,
//									   fingerXYZ + glm::vec3(1.0,0,0),
//									   fingerXYZ - glm::vec3(1.0,0,0),
//									   glm::vec3(1.0f, 1.0f, 1.0f) );
//// *********************************************************************************

//    ___         __                         _   ___                _             ___               
//   |   \  ___  / _| ___  _ _  _ _  ___  __| | | _ \ ___  _ _   __| | ___  _ _  | _ \ __ _  ___ ___
//   | |) |/ -_)|  _|/ -_)| '_|| '_|/ -_)/ _` | |   // -_)| ' \ / _` |/ -_)| '_| |  _// _` |(_-<(_-<
//   |___/ \___||_|  \___||_|  |_|  \___|\__,_| |_|_\\___||_||_|\__,_|\___||_|   |_|  \__,_|/__//__/
//   
// In this pass, we READ from the G-buffer, and calculate the lighting. 
// In this example, we are writing to another FBO, now. 
// 


		// Render it again, but point the the FBO texture... 
	

glBindFramebuffer(GL_FRAMEBUFFER, pass2_cFBO.ID);
		pass2_cFBO.clearBuffers();
// ************************************
		// SETS THE CLEAR SCREEN COLOUR TO RED (to show outline of light volume)
		//pass2_cFBO.clearBuffers(glm::vec4(1.0f, 0.0, 0.0, 1.0f));
// ************************************


		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		
		
		//CHANGES STENCIL COLOR
		//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		
		
		
		
		glDisable(GL_STENCIL_TEST);

		::g_pShaderManager->useShaderProgram("mySexyShader");

		glUniform1i( shaderPassID, PassInt1 );
		
		//uniform sampler2D texFBONormal2D;
		//uniform sampler2D texFBOVertexWorldPos2D;

		GLint texFBOColour2DTextureUnitID = 10;
		GLint texFBOColour2DLocID = glGetUniformLocation(sexyShaderID, "texFBOColour2D");
		GLint texFBONormal2DTextureUnitID = 11;
		GLint texFBONormal2DLocID = glGetUniformLocation(sexyShaderID, "texFBONormal2D");
		GLint texFBOWorldPosition2DTextureUnitID = 12;
		GLint texFBOWorldPosition2DLocID = glGetUniformLocation(sexyShaderID, "texFBOVertexWorldPos2D");

		// Pick a texture unit... 
		glActiveTexture(GL_TEXTURE0 + texFBOColour2DTextureUnitID);
		glBindTexture(GL_TEXTURE_2D, pass1_cFBO.colourTexture_0_ID);
		glUniform1i(texFBOColour2DLocID, texFBOColour2DTextureUnitID);

		glActiveTexture(GL_TEXTURE0 + texFBONormal2DTextureUnitID);
		glBindTexture(GL_TEXTURE_2D, pass1_cFBO.normalTexture_1_ID);
		glUniform1i(texFBONormal2DLocID, texFBONormal2DTextureUnitID);
		
		glActiveTexture(GL_TEXTURE0 + texFBOWorldPosition2DTextureUnitID);
		glBindTexture(GL_TEXTURE_2D, pass1_cFBO.vertexWorldPos_2_ID);
		glUniform1i(texFBOWorldPosition2DLocID, texFBOWorldPosition2DTextureUnitID);
		
		// Set the sampler in the shader to the same texture unit (20)

		glfwGetFramebufferSize(::g_pGLFWWindow, &width, &height);

		GLint screenWidthLocID = glGetUniformLocation(sexyShaderID, "screenWidth");
		GLint screenHeightLocID = glGetUniformLocation(sexyShaderID, "screenHeight");
		glUniform1f(screenWidthLocID, (float)width);
		glUniform1f(screenHeightLocID, (float)height);

		// Adjust the CA:
		//float g_ChromaticAberrationOffset = 0.0f;
		// 0.1
		if ( ::g_CA_CountDown > 0.0 )
		{
			float CRChange = getRandInRange( (-g_CR_Max/10.0f), (::g_CR_Max/10.0f) );
			::g_ChromaticAberrationOffset += CRChange;
			::g_CA_CountDown -= ::deltaTime;
		}
		else
		{
			::g_ChromaticAberrationOffset = 0.0f;
		}

		GLint CROffset_LocID = glGetUniformLocation(sexyShaderID, "CAoffset" );
		glUniform1f( CROffset_LocID, g_ChromaticAberrationOffset);

		std::vector< cGameObject* >  vecLightAccumulationPass;
		// Push back a SINGLE quad or GIANT triangle that fills the entire screen
		// Here we will use the skybox (as it fills the entire screen)
		//vecLightAccumulationPass.push_back( ::g_pSkyBoxObject );
//		cGameObject* pBunny = findObjectByFriendlyName("bugs", ::g_vecGameObjects);
//		vecCopy2ndPass.push_back(pBunny);
		

		// Use the distance calculation to determine how bit the light sphere
		//	object is going to be to draw...

		cLight &Light0 = ::g_pLightManager->vecLights[0];

		// HACK: Sets the light location and attenuation once, then we can move it...
		if ( ! bHACK_RUN_ONCE_TO_SET_LIGHT)
		{
			Light0.position = glm::vec3(0.0f, 100.0f, -180.0f);
			Light0.attenuation.x = 0.0f;
			Light0.attenuation.y = 0.00330822f;
			Light0.attenuation.z = 0.00507976f;
			bHACK_RUN_ONCE_TO_SET_LIGHT = true;
		}
		// This looks nice: 0, 100, -180 dist = 136.719atten: 0:0.00330822:0.00507976


		float HowMuchLightIsEffectivelyDark = 0.01f;		// 1 percent?
		float lightEffectDistance 
			= Light0.calcApproxDistFromAtten(HowMuchLightIsEffectivelyDark,
											 0.001f );		// Within 0.1 percent?

		//  myLight[lightID].typeParams.y is lightDistance
		Light0.typeParams.y = lightEffectDistance;

		g_pLightSphere2Sided->vecMeshes[0].scale = lightEffectDistance;

		// Set this sphere where the light is...
		cPhysProps LightSpherePys = ::g_pLightSphere2Sided->GetPhysState();
		LightSpherePys.position = ::g_pLightManager->vecLights[0].position;

		::g_pLightSphere2Sided->SetPhysState(LightSpherePys);
		/*std::cout 
			<< Light0.position.x << ", "
			<< Light0.position.y << ", "
			<< Light0.position.z
			<< " dist = " << lightEffectDistance 
			<< "atten: " 
			<< Light0.attenuation.x << ":"
			<< Light0.attenuation.y << ":" 
			<< Light0.attenuation.z << std::endl;*/
		//*change
		//vecLightAccumulationPass.push_back(::g_pLightSphere2Sided);
		vecLightAccumulationPass.push_back(::g_pSkyBoxObject);
	//	glUniform( WhichLightIsThisID );
		RenderScene(vecLightAccumulationPass, ::g_pGLFWWindow, ::deltaTime );

	

		// Set the scissor buffer
//// Example to render only INSIDE the scissor buffer (square) area
//		glScissor(500, 500, 1080, 600);
//		glEnable(GL_SCISSOR_TEST );
//		RenderScene(vecCopy2ndPass, ::g_pGLFWWindow, ::deltaTime );
//		glDisable(GL_SCISSOR_TEST );
//// 
//
//// Example to render only OUTSIDE of the scissor area:
//		RenderScene(vecCopy2ndPass, ::g_pGLFWWindow, ::deltaTime );
//		glScissor(500, 500, 1080, 600);
//		glEnable(GL_SCISSOR_TEST );
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glDisable(GL_SCISSOR_TEST );
//// 


	
		

//    ___  _              _   ___  ___    ___               
//   | __|(_) _ _   __ _ | | |_  )|   \  | _ \ __ _  ___ ___
//   | _| | || ' \ / _` || |  / / | |) | |  _// _` |(_-<(_-<
//   |_|  |_||_||_|\__,_||_| /___||___/  |_|  \__,_|/__//__/
//                                                          	
// Here, we read from the off screen buffer, the one that 
// has all the lighting, etc. 
// This is where we can do the "2nd pass effects", so the 
// full-screen 2D effects.
//
// NOTE: In this example, we are only using this to render to an offscreen object

		// Now the final pass (in this case, only rendering to a quad)
		//PassInt2
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		
		//GLint renderPassNumber_LocID = glGetUniformLocation(sexyShaderID, "prakash");
		//glUniform1i(renderPassNumber_LocID, 0);
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

		
		
		
		
		::g_pShaderManager->useShaderProgram("mySexyShader");

		glUniform1i(shaderPassID, 2 );

		/// The "deferred pass" FBO has a colour texture with the entire rendered scene
		/// (including lighting, etc.)
		GLint fullRenderedImage2D_LocID = glGetUniformLocation(sexyShaderID, "fullRenderedImage2D");

		/// Pick a texture unit... 
		unsigned int pass2unit = 50;
		glActiveTexture( GL_TEXTURE0 + pass2unit);
		glBindTexture(GL_TEXTURE_2D, ::pass2_cFBO.colourTexture_0_ID);
		glUniform1i(fullRenderedImage2D_LocID, pass2unit);


		std::vector< cGameObject* >  vecCopySingleLonelyQuad;
		/// Push back a SINGLE quad or GIANT triangle that fills the entire screen
//		vecCopySingleLonelyQuad.push_back( ::g_ExampleTexturedQuad );
//		cGameObject* pTheShip = findObjectByFriendlyNam( "NCC-1701", ::g_vecGameObjects );
//		vecCopySingleLonelyQuad.push_back( pTheShip );
		
		
		
		
		
		vecCopySingleLonelyQuad.push_back( ::g_pSkyBoxObject );
		RenderScene(vecCopySingleLonelyQuad, ::g_pGLFWWindow, ::deltaTime);





//		RenderScene(::g_vecTransparentObjectsOnly, ::g_pGLFWWindow, ::deltaTime);

		std::stringstream ssTitle;
//		ssTitle << "Camera (xyz): " 
//			<< g_cameraXYZ.x << ", " 
//			<< g_cameraXYZ.y << ", " 
//			<< g_cameraXYZ.z;
		// 
//		glm::vec4 EulerAngle;
		//g_pTheCamera->qOrientation.eularAngles(EulerAngle);
		//ssTitle << "Camera (xyz): "
		//	<< EulerAngle.x << ", "
		//	<< EulerAngle.y << ", "
		//	<< EulerAngle.z;

		glfwSetWindowTitle( ::g_pGLFWWindow, ssTitle.str().c_str() );

		// "Presents" what we've drawn
		// Done once per scene 
        glfwSwapBuffers(::g_pGLFWWindow);
        glfwPollEvents();


    }// while ( ! glfwWindowShouldClose(window) )


    glfwDestroyWindow(::g_pGLFWWindow);
    glfwTerminate();

	// 
	delete ::g_pShaderManager;
	delete ::g_pVAOManager;


//	::g_pDalekManager->KillAllDaleks();

//    exit(EXIT_SUCCESS);
	return 0;
}


void DrawDebugLightingSpheres(void)
{
	//DEBUG sphere
	::g_pDebugRenderer->addDebugSphere( glm::vec3( 0.0f, 0.0f, 0.0f ), 
										glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), 1.0f );
	// Light at 95% 
	float scaleAt99 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.99f );	
	::g_pDebugRenderer->addDebugSphere( ::g_pLightManager->vecLights[0].position, 
										glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), scaleAt99 );

	// Light at 50% 
	float scaleAt50 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.5f );	
	::g_pDebugRenderer->addDebugSphere( ::g_pLightManager->vecLights[0].position, 
										glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), scaleAt50 );

	// Light at 25% 
	float scaleAt25 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.25f );	
	::g_pDebugRenderer->addDebugSphere( ::g_pLightManager->vecLights[0].position, 
										glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), scaleAt25 );

	// Light at 1% 
	float scaleAt01 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.01f );	
	::g_pDebugRenderer->addDebugSphere( ::g_pLightManager->vecLights[0].position,glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), scaleAt01 );
	return;
}



void setWindowFullScreenOrWindowed( GLFWwindow* pTheWindow, bool IsFullScreen )
{
	// Set full-screen or windowed
	if ( ::g_IsWindowFullScreen )
	{	
		// Find the size of the primary monitor
		GLFWmonitor* pPrimaryScreen = glfwGetPrimaryMonitor();
		const GLFWvidmode* pPrimMonVidMode = glfwGetVideoMode( pPrimaryScreen );
		// Set this window to full screen, matching the size of the monitor:
		glfwSetWindowMonitor( pTheWindow, pPrimaryScreen, 
							  0, 0,				// left, top corner 
							  pPrimMonVidMode->width, pPrimMonVidMode->height, 
							  GLFW_DONT_CARE );	// refresh rate

		std::cout << "Window now fullscreen at ( " 
			<< pPrimMonVidMode->width << " x " 
			<< pPrimMonVidMode->height << " )" << std::endl;
	}
	else
	{
		// Make the screen windowed. (i.e. It's CURRENTLY full-screen)
		// NOTE: We aren't saving the "old" windowed size - you might want to do that...
		// HACK: Instead, we are taking the old size and mutiplying it by 75% 
		// (the thinking is: the full-screen switch always assumes we want the maximum
		//	resolution - see code above - but when we make that maximum size windowed,
		//  it's going to be 'too big' for the screen)
		GLFWmonitor* pPrimaryScreen = glfwGetPrimaryMonitor();
		const GLFWvidmode* pPrimMonVidMode = glfwGetVideoMode( pPrimaryScreen );

		// Put the top, left corner 10% of the size of the full-screen
		int topCornerTop = (int)( (float)pPrimMonVidMode->height * 0.1f );
		int topCornerLeft = (int)( (float)pPrimMonVidMode->width * 0.1f );
		// Make the width and height 75% of the full-screen resolution
		int height = (int)( (float)pPrimMonVidMode->height * 0.75f );
		int width = (int)( (float)pPrimMonVidMode->width * 0.75f );

		glfwSetWindowMonitor( pTheWindow, NULL,		// This NULL makes the screen windowed
							  topCornerLeft, topCornerTop,				// left, top corner 
							  width, height, 
							  GLFW_DONT_CARE );	// refresh rate

		std::cout << "Window now windowed at ( " 
			<< width << " x " << height << " )" 
			<< " and offset to ( "
			<< topCornerLeft << ", " << topCornerTop << " )"
			<< std::endl;
	}
	return;
}
























