**Linking the Renderer:**
- Add the `Build/Renderer.vcxproj` project to your solution in Visual Studio.
- Add the renderer project's output directory to the *Library Directories* under the client project's *Property Pages > VC++ Directories*. For example: `$(SolutionDir)..\Build\x64\Release` for a release configuration.
- Add `Renderer.lib` to *Additional Dependencies* under the client project's *Property Pages > Linker > Input*.
- Add the directory of `Renderer/RendererAPI.h` to the *Additional Include Directories* under the client project's *Property Pages > C/C++ > General*. For example: `$(SolutionDir)../Renderer`.

**Using the Renderer:**
- Implement `IEntityRenderInfo` for each entity to tell the renderer where and how to render the entity (position, rotation, and scale). These values can be changed each frame.
- Load mesh .obj and texture .png files through `IRenderer` and use their enums in `IEntityRenderInfo` to denote the mesh/texture the renderer should use when drawing that entity.
- Implement `IAccessEntityRenderInfo` to send the set of entities for the renderer to draw.
- Use `IAdjustCamera` to pan, zoom, and move the camera.
- Use the `Render` function from `IRenderer` to tell the renderer when to update the window.
