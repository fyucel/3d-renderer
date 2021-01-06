#pragma once

#ifdef EXPORT_RENDERER
#define RENDERER_API __declspec(dllexport)
#else
#define RENDERER_API __declspec(dllimport)
#endif

// Implemented by entities to tell the renderer how to draw the entity
class RENDERER_API IEntityRenderInfo
{
public:
	virtual int MeshType() const = 0;
	virtual int TextureType() const = 0;

	virtual float PositionX() const = 0;
	virtual float PositionY() const = 0;
	virtual float PositionZ() const = 0;

	virtual float RotationX() const = 0;
	virtual float RotationY() const = 0;
	virtual float RotationZ() const = 0;

	virtual float ScaleX() const = 0;
	virtual float ScaleY() const = 0;
	virtual float ScaleZ() const = 0;
};

// Implemented by the client to access the render info of entities
// that will be drawn by the renderer
class RENDERER_API IAccessEntityRenderInfo
{
public:
	virtual const std::unordered_set<IEntityRenderInfo*> EntitiesToRender() = 0;
};

// Interface exposed to the client for adjusting the camera position/orientation
class RENDERER_API IAdjustCamera
{
public:
	virtual void PanLeft(float secondsElapsed,
		bool fastSpeed = false) = 0;

	virtual void PanRight(float secondsElapsed,
		bool fastSpeed = false) = 0;

	virtual void PanForwards(float secondsElapsed,
		bool fastSpeed = false) = 0;

	virtual void PanBackwards(float secondsElapsed,
		bool fastSpeed = false) = 0;

	virtual void ZoomIn(float secondsElapsed, bool fastSpeed) = 0;
	virtual void ZoomOut(float secondsElapsed, bool fastSpeed) = 0;

	virtual void Move(int offsetX, int offsetY, float secondsElapsed) = 0;
};

// Interface exposed to the client for telling when to render the entities
// and accessing the camera adjustment interface
class RENDERER_API IRenderer
{
public:
	virtual void Render(IAccessEntityRenderInfo* accessEntityRenderInfo) = 0;

	virtual IAdjustCamera* AdjustCamera() = 0;
};

// Creates a new renderer instance, giving ownership to the caller
extern RENDERER_API IRenderer* InitializeRenderer();