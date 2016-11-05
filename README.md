# Particle System
This is a small particle system written in C++11/14 with OpenGL. It uses compute shader to compute the movement of the particles. 
 
## Keys
- W, A, S, D - Moving the camera
- E, Q -  Adjusting the attractor's depth
- Left mouse button  - Activating the attractor mode (Camera does not move anymore)
- Right mouse button -  Activating the attractor as the source of gravity (Only in attractor mode)
- Space - Toggle VSync


## Used Libraries
- gl3w
- GLFW 3.2.1
- GLM

## Errors and Problems

If no GLFW window can be created nor an OpenGL context then make sure you have got the latest graphics driver which supports OpenGL 4.3.

## Comment

I started this project to learn OpenGL. It has been revised several time since the commit. When there are bugs, design problems or inconsistencies please let me know.

You can use this code as you like for every purpose. It would be kind to mention my name "Stanislaw Eppinger" when you use the code.

When you've got any questions regarding this particle system then you may contact me. Thank you for downloading this humble project! =)
