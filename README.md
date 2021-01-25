# 3D Renderer

Abstracted rendering component that can be linked as a DLL to an existing C++ application

---

### Usage:

[`/Example`](https://github.com/fyucel/3d-renderer/tree/master/Example) demonstrates a client application that draws a model that the user can move around.

---

### Demo:

![](Car.gif)
> *Looking at a car model, rendered with GL_FILL (polygon interiors filled)*

![](Teapots.gif)
> *Navigating around teapot models with some moving, rotating, and/or enlarging, rendered with GL_LINE (polygon boundary edges drawn as line segments)*

---

### Library Dependencies:
- [GLEW](http://glew.sourceforge.net/) – OpenGL rendering
- [GLM](https://github.com/g-truc/glm) – mathematics library
- [SDL](https://www.libsdl.org/) – window creation and input polling
- [stb_image.h](https://github.com/nothings/stb) – PNG texture loading
