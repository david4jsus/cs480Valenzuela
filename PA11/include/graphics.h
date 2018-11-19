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
    Camera* GetCamera();
    void SwitchShaders();
    void SetVertexShader();
    void SetFragmentShader();
    
    // To get a specified cube, specified by array index
    Object* GetObject(int index);
	int GetNumberOfObjects();
	 
	// Bullet 
	btDiscreteDynamicsWorld* GetDynamicsWorld();
	 
	float GetAmbientLightingScale();
	float SetAmbientLightingScale(float setAmbientLighting);
	 
	float GetSpecularScale();
	float SetSpecularScale(float setSpecularScale);
	 
	btRigidBody* GetRigidBody(int objectIndex);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_PerVertexShader;
    Shader *m_PerFragmentShader;

    // Per vertex
    GLint m_vprojectionMatrix;
    GLint m_vviewMatrix;
    GLint m_vmodelMatrix;
    GLint m_vlightPos;
    GLint m_vambientColor;
    GLint m_vdiffuseColor;
    GLint m_vspecularColor;
    GLint m_vshininess;
    
    // Per fragment
    GLint m_fprojectionMatrix;
    GLint m_fviewMatrix;
    GLint m_fmodelMatrix;
    GLint m_flightPos;
    GLint m_fambientColor;
    GLint m_fdiffuseColor;
    GLint m_fspecularColor;
    GLint m_fshininess;

    std::vector<Object*> m_objects;

    Sound gameSound;
    
    // Bullet Members
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfig;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
    
    // Shader switching
    bool shaderToggle;
    
    // ambient lighting values
    float ambientLightingScale;
    float specularScale;
    
    glm::vec3 pinballPos;
    
    std::vector<btRigidBody*> rigidBodies;
    
    bool meshLoaded;
};

#endif /* GRAPHICS_H */
