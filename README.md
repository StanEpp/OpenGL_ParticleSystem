# Particle System
This is a small particle system written in C++14/17 with OpenGL. It uses compute shader to compute the movement of the particles.

## Build and Run
Clone the repository:
```
git clone https://github.com/StanEpp/OpenGL_ParticleSystem.git
```
Then build it:
```
cd OpenGL_ParticleSystem
mkdir build
cd build
cmake ..
make -j8
```
Execute the application and run one example scene:
```
./OpenGL_ParticleSystem
```

## Keys
- W, A, S, D - Moving the camera
- E, Q -  Adjusting the attractor's depth
- Left mouse button  - Activating the attractor mode (Camera does not move anymore)
- Right mouse button -  Activating the attractor as the source of gravity (Only in attractor mode)
- TAB - Toggle Fps in title bar. Only visible when in window mode.


## Errors and Problems

If no GLFW window can be created nor an OpenGL context then make sure you've got the latest graphics driver which supports OpenGL 4.3.

## Comment

I started this project to learn OpenGL. It has been revised several times since the first commit. When there are bugs, design problems or inconsistencies please let me know.
