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

	void PanLeft(float secondsElapsed, bool fastSpeed = false) override;
	void PanRight(float secondsElapsed, bool fastSpeed = false) override;
	void PanForwards(float secondsElapsed, bool fastSpeed = false) override;
	void PanBackwards(float secondsElapsed, bool fastSpeed = false) override;
	void ZoomIn(float secondsElapsed, bool fastSpeed) override;
	void ZoomOut(float secondsElapsed, bool fastSpeed) override;
	void Move(int offsetX, int offsetY, float secondsElapsed) override;

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
	static const float NormalPanSpeed;
	static const float FastPanSpeed;
	static const float NormalZoomSpeed;
	static const float FastZoomSpeed;

	static const float LightSourceYPosition;
};