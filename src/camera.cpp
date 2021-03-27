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
	return glm::lookAt({0,0,0}, getViewDirection(), upVector);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(fovRadians,
		aspectRatio, closePlane, farPlane);
}

glm::vec3 Camera::getViewDirection()
{
	glm::vec3 viewDirection = {0,0,-1};

	//now we rotate by y vector
	viewDirection = glm::mat3(glm::rotate(viewAngle.x, upVector)) * viewDirection;

	glm::vec3 vectorToTheRight = glm::cross(viewDirection, upVector);

	//now we rotate by x vector
	viewDirection = glm::mat3(glm::rotate(viewAngle.y, vectorToTheRight)) * viewDirection;


	viewDirection = glm::normalize(viewDirection);

	return viewDirection;
}

void Camera::rotateCamera(float x, float y)
{
	if(!x && !y)
	{
		return;
	}

	x *= -1;
	y *= -1;

	//float speed = glm::radians(10.f);
	float speed = 0.10;
	
	viewAngle += glm::vec2{x, y} *speed;

	viewAngle.y = glm::clamp(viewAngle.y, glm::radians(-89.f), glm::radians(89.f));

}

void FirstPersonFlyCamera::move(glm::vec3 direction)
{

	glm::vec3 m = {};
	m.y = direction.y;

	glm::vec3 moveFront = direction;

	if (direction.x || direction.z)
	{
		glm::vec3 v = getViewDirection();
		v.y = 0;
		m -= glm::normalize(v) * direction.z;
		m += glm::normalize(glm::cross(v, glm::vec3{ 0,1,0 })) * direction.x;
	}


	position += m;

}
