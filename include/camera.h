#pragma once
#include <glm/glm.hpp>

class Camera
{
public:

	glm::vec3 &getPosition(){return position;}

	//full view transform matrix
	glm::mat4 getProjectionViewMatrix(); 
	
	//view matrix
	glm::mat4 getViewMatrix(); 
	
	//just the projection matrix
	glm::mat4 getProjectionMatrix(); 

	void rotateCamera(glm::vec2 v) { rotateCamera(v.x, v.y); }
	void rotateCamera(float x, float y);

	void updateAspectRatio(float w, float h)
	{
		aspectRatio = w / h;
	}

private:

	glm::vec3 position = {};
	glm::vec3 upVector = { 0,1,0 };
	glm::vec3 viewDirection = { 0,0,-1 };

	float fovRadians = glm::radians(100.f);
	float closePlane = 0.1;
	float farPlane = 100;
	float aspectRatio = 1.f;

};