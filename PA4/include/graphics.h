#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, std::string file);
    void Update(unsigned int dt);
    void Render();
    
    // To get a specified cube, specified by array index
    Object* getCube(int index);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    //Object *m_cube;
    std::vector<Object*> m_cubes;
};

#endif /* GRAPHICS_H */
