#pragma once
#include <glad/glad.h>
#include <globalOpenGL_GLFW.h>
#include <iostream>			
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>		
#include <string>
#include <vector>
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include<iostream>
#include<vector>

class PhysicsDefinitions
{

public:

	struct force
	{
		bool on = 0;
		float strength = 0.0f;
		glm::vec3 direction = glm::vec3(0,0,0);

	};

	
	
	enum PhysicsShapes
	{	

		SPHERE,		
		PLANE,		
		CAPSULE,    
		AABB,		
		UNKNOWN,
		BOX,
		RECTANGLE

	};

	struct PhysicsObject
	{
		std::string ObjectName;

		glm::vec3 PhysicsPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat PhysicsOrientation1 = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
		glm::quat PhysicsOrientation2 = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 PhysicsManualVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

		PhysicsShapes type = PhysicsShapes::UNKNOWN;

		float PhysicsScale = 1;
		float Mass = 10;

		bool ApplyPhysics = 1;
		bool CollisionDetected = 0;

	};

	private:


};

