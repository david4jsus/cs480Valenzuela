# PA6: Assimp Model Loading with Textures

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

Thi project alos uses ImageMagick.

## Building and Running
To build this project generated the make file using CMake. Navigate to the build folder to build the program. To create the program, simply run the "cmake .." than the "make" command. Then, run the program.

```bash
cd build
cmake ..
make
./Tutorial
```

# Loading a model

By default, the project loads a cibe object, or in the case that a specified model could not be loaded. To load your own model, place the '**.obj**' file in the '**assets/models/**' subdirectory of the project. Then, when running the program, specify the name of the file as a command line argument (please include '.obj' in the name of the file):

```bash
./Tutorial buddha.obj
```

If there is a file called 'object.obj' in the 'models/' file, it will load that file. If no file is provided, program will terminate.

# Controls

To reverse the direction of rotation of the object, click on the screen or press the '**A**' key. To make the object rotate specifically in a counter-clockwise direction, press the **left arrow** button. To make it rotate specifically in a clockwise direction, press the **right arrow** button. You can also stop the object from orbiting by pressing the '**S**' key, and stop it from rotating by pressing the '**D**' key. To quit the program, press the **escape button**. There will be an on-screen menu indicating the diretion of rotation of the object.
