#include "Utilities.h"
#include "Camera.h"

Camera::Camera() :
	viewMatrix{ 1.0f },
	projectionMatrix{ 1.0f },
	front{ 0.0f, 0.0f, -1.0f },
	right{ 0.0f },
	up{ 0.0f, 1.0f, 0.0f },
	position{ 0.0f },
	worldUp{ 0.0f, 1.0f, 0.0f },
	lightSource{ 0.0f, LightSourceYPosition, 0.0f },
	pitch{ 0.0f },
	yaw{ -90.0f },
	fieldOfView{ DefaultFieldOfView },
	nearPlane{ DefaultNearPlane },
	farPlane{ DefaultFarPlane },
	aspectRatio{ 0 } {}

const glm::vec3& Camera::Position() const
{
	return position;
}

const glm::mat4& Camera::ViewMatrix() const
{
	return viewMatrix;
}

const glm::mat4& Camera::ProjectionMatrix() const
{
	return projectionMatrix;
}

const glm::vec3& Camera::LightSource() const
{
	return lightSource;
}

void Camera::Update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	viewMatrix = glm::lookAt(position,
		position + front, up);
}

void Camera::AdjustProjection(int windowWidth, int windowHeight)
{
	aspectRatio = (float)windowWidth / windowHeight;

	projectionMatrix = glm::perspective(glm::radians(fieldOfView),
		aspectRatio, nearPlane, farPlane);
}

void Camera::ScrollLeft(float secondsElapsed, bool fastSpeed)
{
	position -= (fastSpeed ? FastScrollSpeed : NormalScrollSpeed)
		* secondsElapsed * glm::normalize(glm::cross(front, up));
}

void Camera::ScrollRight(float secondsElapsed, bool fastSpeed)
{
	position += (fastSpeed ? FastScrollSpeed : NormalScrollSpeed)
		* secondsElapsed * glm::normalize(glm::cross(front, up));
}

void Camera::ScrollForwards(float secondsElapsed, bool fastSpeed)
{
	// Do not change the position.z
	auto result = (fastSpeed ? FastScrollSpeed : NormalScrollSpeed)
		* secondsElapsed * front;
	position.x += result.x;
	position.z += result.z;
}

void Camera::ScrollBackwards(float secondsElapsed, bool fastSpeed)
{
	// Do not change the position.z
	auto result = (fastSpeed ? FastScrollSpeed : NormalScrollSpeed)
		* secondsElapsed * front;
	position.x -= result.x;
	position.z -= result.z;
}

void Camera::ZoomIn(float secondsElapsed, bool fastSpeed)
{
	position.y += (fastSpeed ? FastZoomSpeed : NormalZoomSpeed)
		* secondsElapsed;
}

void Camera::ZoomOut(float secondsElapsed, bool fastSpeed)
{
	position.y -= (fastSpeed ? FastZoomSpeed : NormalZoomSpeed)
		* secondsElapsed;
}

void Camera::Move(float offsetX, float offsetY, float secondsElapsed)
{
	yaw += offsetX * secondsElapsed * MoveSpeed;
	pitch += offsetY * secondsElapsed * MoveSpeed;

	if (pitch > 80) pitch = 80.0f;
	else if (pitch < -80) pitch = -80.0f;

	if (yaw > 360 || yaw < -360) yaw = 0.0f;
}

const float Camera::DefaultFieldOfView = 90.0;
const float Camera::DefaultNearPlane = 0.1f;
const float Camera::DefaultFarPlane = 1000.0f;

const float Camera::MoveSpeed = 10.0f;
const float Camera::NormalScrollSpeed = 5.0f;
const float Camera::FastScrollSpeed = 15.0f;
const float Camera::NormalZoomSpeed = 50.0f;
const float Camera::FastZoomSpeed = 100.0f;

const float Camera::LightSourceYPosition = 1000.0f;