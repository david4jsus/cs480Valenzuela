# PA11: Group Project - Joust

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

Thi project also uses the Assimp, ImageMagick, and Bullet physics libraries.

## Building and Running
To build this project generated the make file using CMake. Navigate to the build folder to build the program. To create the program, simply run the "cmake .." than the "make" command. Then, run the program.

```bash
mkdir build
cd build
cmake ..
make
./Joust
```

# Controls
This program uses keyboard input and the mouse to select items on the on-screen menu:

Moving the camera

Hold left click and move mouse left: look left
Hold left click and move mouse right: look right
Hold left click and move mouse up: look down
Hold left click and move mouse down: look up
Hold shift and move mouse left: move left
Hold shift and move mouse right: move right
Hold shift and move mouse down: move forward
Hold shift and move mouse up: move backward


Move Player One

- **I**: Move cube forward
- **J**: Move cube left
- **K**: Move cube backward
- **L**: Move cube right

Move Player Two

- **T**: Move cube forward
- **F**: Move cube left
- **G**: Move cube backward
- **H**: Move cube right

Changing ambient and specular brightness

- **Numpad 8**: Increase ambient brightness
- **Numpad 2**: Decrease ambient brightness
- **Numpad 4**: Decrease specular brightness
- **Numpad 6**: Increase specular brightness

The on-screen menu is scalable and has an available scrolling bar on the right side.
