#include "camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

glm::mat4 Camera::getProjectionViewMatrix()
{
	return getProjectionMatrix() * getViewMatrix();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + viewDirection, upVector);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(fovRadians,
		aspectRatio, closePlane, farPlane);
}

//https://github.com/meemknight/minecraftGl/blob/5f7334e682765350c726bf9c8abbd333104fe9cb/minecraftGl/Camera.cpp#L150
void Camera::rotateCamera(float x, float y)
{
	x *= -1;
	y *= -1;

	float speed = 2;

	glm::vec3 vectorToTheRight = glm::cross(viewDirection, upVector);

	glm::vec3 oldPos = viewDirection;


	//now we rotate by y vector
	viewDirection = glm::mat3(glm::rotate(glm::radians(x * speed), upVector)) * viewDirection;
	
	auto isPositive = [](float n){return n >= 0;};
	glm::vec3 tiltTest = glm::mat3(glm::rotate(glm::radians(x * speed), vectorToTheRight)) * viewDirection;
	
	//first check if we are not already looking all the way up or down
	if (isPositive(oldPos.x) == !isPositive(tiltTest.x) &&
		isPositive(oldPos.z) == !isPositive(tiltTest.z))
	{
	
	}
	else
	{
		viewDirection = glm::mat3(glm::rotate(glm::radians(y * speed), vectorToTheRight)) * viewDirection;
	}

	viewDirection = glm::normalize(viewDirection);
}
