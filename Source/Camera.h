#pragma once

#include "RendererAPI.h"

class Camera : public IAdjustCamera
{
public:
	Camera();

	const glm::vec3& Position() const;
	const glm::mat4& ViewMatrix() const;
	const glm::mat4& ProjectionMatrix() const;
	const glm::vec3& LightSource() const;

	void Update();
	void AdjustProjection(int windowWidth, int windowHeight);

	void ScrollLeft(float secondsElapsed, bool fastSpeed = false) override;
	void ScrollRight(float secondsElapsed, bool fastSpeed = false) override;
	void ScrollForwards(float secondsElapsed, bool fastSpeed = false) override;
	void ScrollBackwards(float secondsElapsed, bool fastSpeed = false) override;
	void ZoomIn(float secondsElapsed, bool fastSpeed) override;
	void ZoomOut(float secondsElapsed, bool fastSpeed) override;
	void Move(float offsetX, float offsetY, float secondsElapsed) override;

private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 front, right, up;
	glm::vec3 position;
	glm::vec3 worldUp;
	glm::vec3 lightSource;

	float pitch, yaw;

	float fieldOfView; // degrees
	static const float DefaultFieldOfView; // degrees

	float nearPlane, farPlane;
	static const float DefaultNearPlane;
	static const float DefaultFarPlane;

	float aspectRatio; // %

	static const float MoveSpeed;
	static const float NormalScrollSpeed;
	static const float FastScrollSpeed;
	static const float NormalZoomSpeed;
	static const float FastZoomSpeed;

	static const float LightSourceYPosition;
};