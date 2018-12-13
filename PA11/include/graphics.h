#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "physics.h"
#include "object.h"
#include "sound.h"
#include "objectInfo.h"
#include "playerSettings.h"

#include <btBulletDynamicsCommon.h>

class Physics;
class Object;

class Graphics
{
  public:
    Graphics();
    Graphics(string vLightingVertFilePath, string vLightingFragFilePath, string fLightingVertFilePath, string fLightingFragFilePath, glm::vec3 storedEngineStartingCameraPos, 
             float storedEngineYaw, float storedEnginePitch, btVector3 gravityDirection, float jumpHeight, std::vector<ObjectInfo> allObjectsInfo);
    ~Graphics();
    bool Initialize(int width, int height, std::string file);
    void Update(unsigned int dt);
    void Render();
    Camera* GetCamera();
    void SwitchShaders();
    void SetVertexShader();
    void SetFragmentShader();
    
    // To get a specified object, specified by array index
    Object* GetObject(int index);
    
    Object* GetObjectByName(string name);
    
		int GetNumberOfObjects();
		 
		// Physics 
		btDiscreteDynamicsWorld* GetDynamicsWorld();
		 
		float GetAmbientLightingScale();
		float SetAmbientLightingScale(float setAmbientLighting);
		 
		float GetSpecularScale();
		float SetSpecularScale(float setSpecularScale);
		 
		btRigidBody* GetRigidBody(string objectName);
	
		// sets the initial starting camera position
		void setCameraStartingPos();

		// sets current player conditions such as lives remaining
		void setPlayerSettings(PlayerSettings* players);

		// gets gravity direction
		btVector3 getGravity();

		// gets jump height
		float getJumpHeight();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_PerVertexShader;
    Shader *m_PerFragmentShader;
    Physics *m_physics;

    // Per vertex
    string vLightingVertexShaderFilePath;
    string vLightingFragmentShaderFilePath;
    GLint m_vprojectionMatrix;
    GLint m_vviewMatrix;
    GLint m_vmodelMatrix;

		// first light
    GLint m_vFirstLightPos;
    GLint m_vambientColor;
    GLint m_vFirstLightDiffuseColor;
    GLint m_vFirstLightSpecularColor;
    GLint m_vFirstLightShininess;

		// second light
		GLint m_vSecondLightPos;
    GLint m_vSecondLightDiffuseColor;
    GLint m_vSecondLightSpecularColor;
    GLint m_vSecondLightShininess;

		// third light
	  GLint m_vThirdLightPos;
    GLint m_vThirdLightDiffuseColor;
    GLint m_vThirdLightSpecularColor;
    GLint m_vThirdLightShininess;
    
    // Per fragment
    string fLightingVertexShaderFilePath;
    string fLightingFragmentShaderFilePath;
    GLint m_fprojectionMatrix;
    GLint m_fviewMatrix;
    GLint m_fmodelMatrix;

		// first light
    GLint m_fFirstLightPos;
    GLint m_fambientColor;
    GLint m_fFirstLightDiffuseColor;
    GLint m_fFirstLightSpecularColor;
    GLint m_fFirstLightShininess;

		// second light
    GLint m_fSecondLightPos;
		GLint m_fSecondLightDiffuseColor;
    GLint m_fSecondLightSpecularColor;
    GLint m_fSecondLightShininess;

		// third light
		GLint m_fThirdLightPos;
		GLint m_fThirdLightDiffuseColor;
    GLint m_fThirdLightSpecularColor;
    GLint m_fThirdLightShininess;
		
		// game sound
    Sound gameSound;

		// objects
		std::vector<Object*> m_objects;
    
    // Shader switching
    bool shaderToggle;
    
    // ambient lighting values
    float ambientLightingScale;
    float specularScale;
    
    bool meshLoaded;
    
    // camera starting position
    glm::vec3 storedGraphicsStartingCameraPos;
    
    // camera starting orientation
    float storedGraphicsYaw;
    float storedGraphicsPitch;
    
    // information on all objects
    std::vector<ObjectInfo> objectsInfo;
    
    // Light positions
    glm::vec3 light1Pos;
    glm::vec3 light2Pos;

		// gravity direction
		btVector3 gravityDirection;

		// jump height
		float jumpHeight;
};

#endif /* GRAPHICS_H */
