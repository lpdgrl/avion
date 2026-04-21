## DevLog

**2026-04-20**
- Added loading presets from ini file (ImGui)

**2026-04-10**
- Added combine calculations ligths (direction light, point light, spot light)

**2026-04-05**
- Select is added (texture or prefam material)

**2026-03-29**
- Added resource manager

**2026-03-17**
- Added diffuse and specular map to Materias

**2026-03-15**  wa
- Added prefab of materials (metals, plastic, rubber) [Taken here](http://devernay.free.fr/cours/opengl/materials.html)
- Added basic subsystem Texture 

**2026-03-14**
- Added Material/Light subsystems 

**2026-03-12**
- Added devlog to git.
- Gui func which added new object to scene is refactoring. Switch only variable parameters to struct 
parameters.

**2026-02-21**
- Added macros of logging. Logger is refactoring. Logger has become simple and static class.

**2026-01-22**
- Added class pipeline for data is filling to framebuffers (using struct RenderContext)
- Added struct ShaderLight and ShaderObject for comfortable transfer data from gui to shader and render

**2026-01-21**
- Common small improvements
- Added class widget (ImGui) for control parameters of shaders and objects

**2026-01-19**
- Added stupid picker objects (More bugs!!!)

**2026-01-18**
- Added names types of objects in widget (ImGui) 
- Added bash scripts for building and run Engine
- Added func convert pixels of cursor to NDC coordinates (OpenGL)

**2026-01-13**
- Added namespaces

**2026-01-07**
- Calculate FPS and delay. Display them in GUI.

**2026-01-05** 
- Changed InputFloat to SliderFloat in GUI Object

**2026-01-04**
- Added simple listbox of objects
- Implemented adding new object to listbox
- Implemented pick object from listbox
- Binding object parameters by ID from listbox and chage it's 

