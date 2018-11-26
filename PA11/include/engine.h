#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"
#include "imgui.h"
#include "sound.h"

#include "objectInfo.h"

#include <btBulletDynamicsCommon.h>

class Engine
{
  public:
    Engine(string name, int width, int height);
    ~Engine();
    bool Initialize();
    void Run();
    void Keyboard();
    void Mouse();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    
    btRigidBody* GetObjectRigidBody(string objectName);
    
    void loadConfigurationFileInfo();
  
  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;

    Sound gameSound;
    
    // Show Dear ImGui demo window
    bool imgui_demo;

    // Model loading
    std::string m_file;
    
    // Keyboard handling
    bool movingRight;
    bool movingLeft;
    bool movingForward;
    bool movingBackward;
    bool movingUp;
    bool movingDown;
    bool rotatingLeft;
    bool rotatingRight;
    bool rotatingUp;
    bool rotatingDown;
    
    // Player one movement handling
    bool playerOneMoveForward;
    bool playerOneMoveBackward;
    bool playerOneMoveLeft;
    bool playerOneMoveRight;
    
    // Player one movement handling
    bool playerTwoMoveForward;
    bool playerTwoMoveBackward;
    bool playerTwoMoveLeft;
    bool playerTwoMoveRight;
    
    // configuration file variables
      // per vertex lighting
    string storedVLightingVertexShaderFilePath;
    string storedVLightingFragmentShaderFilePath;
    
      // per fragment lighting
    string storedFLightingVertexShaderFilePath;
    string storedFLightingFragmentShaderFilePath;
    
      // camera starting position
    glm::vec3 storedEngineStartingCameraPos;
    
      // camera starting orientation
    float storedEngineYaw;
    float storedEnginePitch;
    
      // information on all objects
    std::vector<ObjectInfo> allObjectsInfo;
};

#endif // ENGINE_H
