#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"
#include "input.h"
#include "imgui.h"
#include "sound.h"
#include "objectInfo.h"
#include "playerSettings.h"

#include <btBulletDynamicsCommon.h>

class Engine
{
  public:
    Engine(string name, int width, int height);
    ~Engine();
    bool Initialize();
    void Run();
    //void Keyboard();
    //void Mouse();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    
    btRigidBody* GetObjectRigidBody(string objectName);
    
    void loadConfigurationFileInfo();

		void setPlayerSettings();
  
    SDL_Event m_event;
    bool m_running;
    unsigned int m_DT;

		void restartGame();
    
  private:
  
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;

    // Graphics related variables
    Graphics *m_graphics;
    long long m_currentTimeMillis;

    // Sound related variables
    Sound gameSound;
    
    // Input related variables
    Input *m_input;
    
    // Show Dear ImGui demo window
    bool imgui_demo;

    // Model loading
    std::string m_file;
    
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
    
    // player settings
    PlayerSettings* players;
};

#endif // ENGINE_H
