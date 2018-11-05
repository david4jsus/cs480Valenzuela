#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sound.h"
#include <btBulletDynamicsCommon.h>

class Object;

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, std::string file);
    void Update(unsigned int dt);
    void Render();
    Camera* getCamera();
    void switchShaders();
    
    // To get a specified cube, specified by array index
    Object* GetObject(int index);
	 int numberOfCubes();
	 
	 // Bullet 
	 btDiscreteDynamicsWorld* GetDynamicsWorld();
	 
	 btRigidBody* rigidBody;

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_PerVertexShader;
    Shader *m_PerFragmentShader;

    GLint m_vprojectionMatrix;
    GLint m_vviewMatrix;
    GLint m_vmodelMatrix;
    GLint m_fprojectionMatrix;
    GLint m_fviewMatrix;
    GLint m_fmodelMatrix;

    //Object *m_cube;
    std::vector<Object*> m_cubes;

    Sound gameSound;
    
    // Bullet Members
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfig;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
    
    // Shader switching
    bool shaderToggle;
};

#endif /* GRAPHICS_H */
