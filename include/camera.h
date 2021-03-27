#pragma once
#include <glm/glm.hpp>

class Camera
{
public:

	glm::vec3 &getPosition() { return position; }

	glm::ivec3 getPositionInWorld()
	{
		glm::ivec3 p = position;

		if (position.x < 0)
		{
			p.x--;
		}

		if(position.y < 0)
		{
			p.y--;
		}

		if(position.z < 0)
		{
			p.z--;
		}

		return p;
	}

	glm::vec3 getViewDirection();

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

	virtual void move(glm::vec3 direction) { position += direction; }


protected:

	glm::vec3 position = {};
	glm::vec3 upVector = { 0,1,0 };

	glm::vec2 viewAngle = {};

	float fovRadians = glm::radians(100.f);
	float closePlane = 0.1f;
	float farPlane = 100.f;
	float aspectRatio = 1.f;

};

class FirstPersonFlyCamera: public Camera
{
public:

	void move(glm::vec3 direction);



};