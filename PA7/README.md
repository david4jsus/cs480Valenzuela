# PA7: Solar System

# Team Specular Bois
The following students are part of the team who made this project:
- Hadi Rumjahn
- David Valenzuela
- Jay Woo

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

This project also uses Dear ImGui (https://github.com/ocornut/imgui), but all the necessary files are already included in the project.

Thi project also uses the Assimp and ImageMagick libraries.

## Building and Running
To build this project generated the make file using CMake. Navigate to the build folder to build the program. To create the program, simply run the "cmake .." than the "make" command. Then, run the program.

```bash
mkdir build
cd build
cmake ..
make
./SolarSystemBois
```

# Controls

Use **WASDQE** to move around and the **arrow keys** to rotate the view. On-screen there will be a menu with different settings for the simulation, such as simulation speed and reversing time.
