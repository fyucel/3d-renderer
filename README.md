## 3D Renderer

Abstracted rendering component that can be linked as a DLL to an existing C++ application

---

**Usage:**
- Implement `IEntityRenderInfo` for each entity to tell the renderer which mesh and texture to use and the entity's position, rotation, and scale
- Implement `IAccessEntityRenderInfo` to send the set of entities to the renderer for it to draw
- Use `IAdjustCamera` from `IRenderer` to adjust the camera position and orientation
- Use `IRenderer` to tell the renderer when to draw the entities from `IAccessEntityRenderInfo` and to load mesh .obj and texture .png files

[`ExampleClient.cpp`](https://github.com/fyucel/3d-renderer/blob/master/Example/ExampleClient.cpp) demonstrates a client application using the renderer API to draw a model that the user can move the camera around

**Dependencies:**
- [GLEW](http://glew.sourceforge.net/) – OpenGL rendering
- [GLM](https://github.com/g-truc/glm) – mathematics library
- [SDL](https://www.libsdl.org/) – window creation and input polling 
