# ThreeBodyProblem

To quote wikipedia: "In physics and classical mechanics, the three-body problem is the problem of taking the initial positions and velocities (or momenta) of three point masses and solving for their subsequent motion according to Newton's laws of motion and Newton's law of universal gravitation."

This project is my take on this problem. 

## Overview
My simulations has almost no regards to physics what so ever because this was not the interesting part for me.

This OpenGL project was focused on trying to imitate computation shaders functionality. Specifically, I wanted to process all the data for drawing the trajectories of bodies to the GPU. This was achieved by using an SSBO that stores the verticies of the trajectories positions. Shader, used for the bodies themselves, updated their trajectories verticies on every call, while the trajectories shader just used the data stored in SSBO. 

GLSL proved to be a little bit unintuitive, though pretty simple to use because of the rather small amount of it's functionality. 

## Usage

The program provides a simple UI. On start, console window appears asking to set the amount of bodies to be generated. I achieved about 15 fps on 2000 bodies. A belive, the limit of the bodies you can generate depends on the size of SSBO's that you hardware supports. 

The starting positions of bodies are randomized in a rather small region. The masses too are randomized. The simulation is supposed to run on 60 fps because is's updates are not based on some fixed delta time or any timer at all. I was too lazy to implement a proper timer to  make the simulation nit frame-rate base
![Alt Text]( https://media.giphy.com/media/yjireAalUoafSxUMKm/giphy.gif) 

## Technologies used 
The visual part of the project was made using OpenGL and various libs based on it:

- C++ STL as the core structure of the project
- OpenGL Extension Wrangler Library (GLEW) for handling OpenGL itself
- OpenGL Mathematics (glm) was used for calculating projections, rotations, moves etc. 
- OpenGL Shading Language (GLSL) was used, surpisingly, for making shaders
- Dear ImGui (ImGui) was used to create the UI

## License
[MIT](https://choosealicense.com/licenses/mit/)
