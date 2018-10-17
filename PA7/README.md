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
This program uses keyboard input for camera interaction and the mouse to select items on the on-screen menu:
- **W**: Move forward
- **S**: Move backward
- **A**: Move left
- **D**: Move right
- **Q**: Move down
- **E**: Move up
- **Up arrow**: Rotate view upwards
- **Down arrow**: Rotate view downwards
- **Left arrow**: Rotate view left
- **Right arrow**: Rotate view right
The on-screen menu is scalable and has an available scrolling bar on the right side.

# Extra Credit
This program features an on-screen menu with options for live adjustment of the simulation speed, as well as a list of astral objects to teleport to. The simulation also features a skybox and rings for the corresponding planets.
