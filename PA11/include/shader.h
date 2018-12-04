#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include "graphics_headers.h"

using namespace std;

class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType, const std::string &fileName);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;
    
    bool LoadShaderSource(const std::string &fileName, std::string &shaderSourceString);
};

#endif  /* SHADER_H */
