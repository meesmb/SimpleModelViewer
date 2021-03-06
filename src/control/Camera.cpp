#include "Camera.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>

Camera::Camera(glm::vec3 initialPos, glm::vec3 up,  GLfloat movementSpeed, int viewPortWidht, int viewPortHeight)
	: pos(initialPos), worldUp(up)
{
	viewPortW = viewPortWidht; viewPortH = viewPortHeight;
	rotate(0, 0);
}


void Camera::rotate(float xChange, float yChange)
{
	glm::vec4 position(pos.x, pos.y, pos.z, 1);
	glm::vec4 pivot(focus.x, focus.y, focus.z, 1);

	// step 1 : Calculate the amount of rotation given the mouse movement.
	float deltaAngleX = 2 * M_PI / viewPortW; // a movement from left to right = 2*PI = 360 deg
	float deltaAngleY = M_PI / viewPortH;  // a movement from top to bottom = PI = 180 deg
	float xAngle = xChange * deltaAngleX;
	float yAngle = yChange * deltaAngleY;

	// Extra step to handle the problem when the camera direction is the same as the up vector
	float cosAngle = glm::dot(getViewDirection(), worldUp);
	//std::cout << yAngle << "\n";
	if (std::abs(cosAngle) > 0.99f && sgn(yChange) == sgn(cosAngle))
		yAngle = 0;

	// step 2: Rotate the camera around the pivot point on the first axis.
	glm::mat4x4 rotationMatrixX(1.0f);
	rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, worldUp);
	position = (rotationMatrixX * (position - pivot)) + pivot;

	// step 3: Rotate the camera around the pivot point on the second axis.
	glm::mat4x4 rotationMatrixY(1.0f);
	rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, getRightVector());
	pos = (rotationMatrixY * (position - pivot)) + pivot;

	calculateViewMatrix();
}

void Camera::setPos(const glm::vec3& p)
{
	this->pos = p;
	calculateViewMatrix();
}

glm::mat4 Camera::getViewMatrix() const
{
	return viewMatrix;
}

void Camera::setFocus(const glm::vec3& f)
{
	this->focus = f;
	calculateViewMatrix();
}

void Camera::calculateViewMatrix()
{
	viewMatrix = glm::lookAt(pos * zoom, focus, worldUp);
}
