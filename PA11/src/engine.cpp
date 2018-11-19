#include "engine.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <fstream>
#include <iostream>

using namespace std;

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  imgui_demo = false;
}

Engine::~Engine()
{
  // Dear ImGui cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // read configuration file
  loadConfigurationFileInfo();

  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics(storedVLightingVertexShaderFilePath, storedVLightingFragmentShaderFilePath, storedFLightingVertexShaderFilePath, storedFLightingFragmentShaderFilePath, 
                            storedEngineStartingCameraPos, storedEngineYaw, storedEnginePitch);
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, m_file))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();
  
  // Setup Dear ImGui binding
  ImGui::CreateContext();
  ImGuiIO& imgui_io = ImGui::GetIO(); (void)imgui_io;
  ImGui_ImplSDL2_InitForOpenGL(m_window->getSDLWindow(), m_window->getGLContext());
  ImGui_ImplOpenGL3_Init("#version 130"); // GL 3.0 + GLSL 130
  ImGui::StyleColorsDark(); // Setup style
  
  // Camera movement stuff
  movingRight    = false;
  movingLeft     = false;
  movingForward  = false;
  movingBackward = false;
  movingUp       = false;
  movingDown     = false;
  rotatingLeft   = false;
  rotatingRight  = false;
  rotatingUp     = false;
  rotatingDown   = false;
  
  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  int index;

  while(m_running)
  { 
    // Update the DT
    m_DT = getDT();

    // Start Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window->getSDLWindow());
    ImGui::NewFrame();
    
    // Check input
    while(SDL_PollEvent(&m_event) != 0)
    {
      ImGui_ImplSDL2_ProcessEvent(&m_event); // Dear ImGui input
      Keyboard();
      Mouse();
    }
    
    // Objects movement
    if (movingLeft)          // Move camera left
    {
      m_graphics->GetCamera()->updateCamPosYNeg(m_DT * 0.05);
    }
    else if (movingRight)    // Move camera right
    {
      m_graphics->GetCamera()->updateCamPosYPos(m_DT * 0.05);
    }
        
    if (movingForward)       // Move camera forward
    {
      m_graphics->GetCamera()->updateCamPosXPos(m_DT * 0.05);
    }
    else if (movingBackward) // Move camera backward
    {
      m_graphics->GetCamera()->updateCamPosXNeg(m_DT * 0.05);
    }
        
    if (movingUp)            // Move camera up
    {
      m_graphics->GetCamera()->updateCamPosZPos(m_DT * 0.05);
    }
    else if (movingDown)     // Move camera down
    {
      m_graphics->GetCamera()->updateCamPosZNeg(m_DT * 0.05);
    }
        
    if (rotatingLeft)        // Rotate camera left
    {
      m_graphics->GetCamera()->updateCamRotYaw(m_DT * -0.1);
    }
    else if (rotatingRight)  // Rotate camera right
    {
      m_graphics->GetCamera()->updateCamRotYaw(m_DT * 0.1);
    }
    
    if (rotatingUp)          // Rotate camera up
    {
      m_graphics->GetCamera()->updateCamRotPitch(m_DT * 0.1);
    }
    else if (rotatingDown)   // Rotate camera down
    {
      m_graphics->GetCamera()->updateCamRotPitch(m_DT * -0.1);
    }
    
    {
      ImGui::Begin("Joust Instructions and Help");
      
      ImGui::Separator();
      ImGui::Separator();
      
      ImGui::Text("Tab -- Switch between Vertex and Fragment Shaders");
      
      ImGui::Separator();
      ImGui::Separator();
      
      ImGui::End();
    }
    
    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render();
    
    // Dear ImGui rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{   
  if (m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // Handle key down events here
    if (m_event.key.keysym.sym == SDLK_ESCAPE) // Quit program
    {
      m_running = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_a)      // Move camera left
    {
      movingLeft  = true;
      movingRight = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_d)      // Move camera right
    {
      movingRight = true;
      movingLeft  = false;
    }
        
    if (m_event.key.keysym.sym == SDLK_w)      // Move camera forward
    {
      movingForward  = true;
      movingBackward = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_s)      // Move camera backward
    {
      movingBackward = true;
      movingForward  = false;
    }
        
    if (m_event.key.keysym.sym == SDLK_e)      // Move camera up
    {
      movingUp   = true;
      movingDown = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_q)      // Move camera down
    {
      movingDown = true;
      movingUp   = false;
    }
        
    if (m_event.key.keysym.sym == SDLK_LEFT)   // Rotate camera left
    {
      rotatingLeft  = true;
      rotatingRight = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_RIGHT)  // Rotate camera right
    {
      rotatingRight = true;
      rotatingLeft  = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_UP)     // Rotate camera up
    {
      rotatingUp   = true;
      rotatingDown = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_DOWN)   // Rotate camera down
    {
      rotatingDown = true;
      rotatingUp   = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_o)   // Rotate camera down
    {
      GetObjectRigidBody(1)->applyCentralImpulse(btVector3(0.0f, 10.0f, 0.0f));
    }
    
    if (m_event.key.keysym.sym == SDLK_i)   // Rotate camera down
    {
      GetObjectRigidBody(1)->applyCentralImpulse(btVector3(1.0f, 0.0f, 0.0f));
    }
    
    if (m_event.key.keysym.sym == SDLK_j)   // Rotate camera down
    {
      GetObjectRigidBody(1)->applyCentralImpulse(btVector3(0.0f, 0.0f, -1.0f));
    }
    
    if (m_event.key.keysym.sym == SDLK_k)   // Rotate camera down
    {
      GetObjectRigidBody(1)->applyCentralImpulse(btVector3(-1.0f, 0.0f, 0.0f));
    }
    
    if (m_event.key.keysym.sym == SDLK_l)   // Rotate camera down
    {
      GetObjectRigidBody(1)->applyCentralImpulse(btVector3(0.0f, 0.0f, 1.0f));
    }
    
    if (m_event.key.keysym.sym == SDLK_TAB) // Switch shaders
    {
    	m_graphics->SwitchShaders();
    }
    
    if(m_event.key.keysym.sym == SDLK_KP_8)
    {
        if(m_graphics->GetAmbientLightingScale() < 1.0)
        {
          m_graphics->SetAmbientLightingScale(m_graphics->GetAmbientLightingScale() + 0.1);
        }
    }
    
    if(m_event.key.keysym.sym == SDLK_KP_2)
    {
        if(m_graphics->GetAmbientLightingScale() > 0.0)
        {
          m_graphics->SetAmbientLightingScale(m_graphics->GetAmbientLightingScale() - 0.1);
        }
    }
    
    // Cheese Specular Scale
    if(m_event.key.keysym.sym == SDLK_KP_6)
    {
        if(m_graphics->GetSpecularScale() < 1.0)
        {
          m_graphics->SetSpecularScale(m_graphics->GetSpecularScale() + 0.1);
        }
    }
    
    if(m_event.key.keysym.sym == SDLK_KP_4)
    {
        if(m_graphics->GetSpecularScale() > 0.0)
        {
          m_graphics->SetSpecularScale(m_graphics->GetSpecularScale() - 0.1);
        }
    }
    
  }
  else if (m_event.type == SDL_KEYUP)
  { 
    if (m_event.key.keysym.sym == SDLK_a)      // Move camera left
    {
      movingLeft = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_d)      // Move camera right
    {
      movingRight = false;
    }
        
    if (m_event.key.keysym.sym == SDLK_w)      // Move camera forward
    {
      movingForward = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_s)      // Move camera backward
    {
      movingBackward = false;
    }
        
    if (m_event.key.keysym.sym == SDLK_e)      // Move camera up
    {
      movingUp = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_q)      // Move camera down
    {
      movingDown = false;
    }
        
    if (m_event.key.keysym.sym == SDLK_LEFT)   // Rotate camera left
    {
      rotatingLeft = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_RIGHT)  // Rotate camera right
    {
      rotatingRight = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_UP)     // Rotate camera up
    {
      rotatingUp = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_DOWN)   // Rotate camera down
    {
      rotatingDown = false;
    }
  }
}

void Engine::Mouse()
{
  if (m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
}

unsigned int Engine::getDT()
{
  endTime = high_resolution_clock::now();
  time_span = endTime - startTime;

  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}

btRigidBody* Engine::GetObjectRigidBody(int objectIndex)
{
  return m_graphics->GetRigidBody(objectIndex);
}

void Engine::loadConfigurationFileInfo()
{
  // local variables
  ifstream fin;
  string configFileInfo;
  float floatInfo;
  
  // clear and open file
  fin.clear();
  fin.open("../assets/configuration_file.txt");
  
  // run until end of file
  while(fin.eof() == false)
  {
    // read in first information segment
    fin >> configFileInfo;
    
    // check if we are about to read in per vertex lighting information
    if(configFileInfo == "Vertex")
    {
      // read in sub-title information
      fin >> configFileInfo;
      fin >> configFileInfo;
      
      // read in vertex shader file path
      fin >> configFileInfo;
      storedVLightingVertexShaderFilePath = configFileInfo;
      
      // read in fragment shader file path
      fin >> configFileInfo;
      storedVLightingFragmentShaderFilePath = configFileInfo;
    }
    
    // check if we are about to read in per fragment lighting information
    else if(configFileInfo == "Fragment")
    {
      // read in sub-title information
      fin >> configFileInfo;
      fin >> configFileInfo;
      
      // read in vertex shader file path
      fin >> configFileInfo;
      storedFLightingVertexShaderFilePath = configFileInfo;
      
      // read in fragment shader file path
      fin >> configFileInfo;
      storedFLightingFragmentShaderFilePath = configFileInfo;
    }
    
    // check if we are about to read in camera starting position information
    else if(configFileInfo == "x-axis:")
    {
      // get camera x-axis starting position
      fin >> floatInfo;
      storedEngineStartingCameraPos.x = floatInfo;
      
      // get camera y-axis starting position
      fin >> configFileInfo;
      fin >> floatInfo;
      storedEngineStartingCameraPos.y = floatInfo;
      
      // get camera z-axis starting position
      fin >> configFileInfo;
      fin >> floatInfo;
      storedEngineStartingCameraPos.z = floatInfo;
    }
    
    // check if we are about to read in camera starting orientation information
    else if(configFileInfo == "yaw:")
    {
      // get camera starting yaw orientation 
      fin >> floatInfo;
      storedEngineYaw = floatInfo;
      
      // get camera starting pitch orientation 
      fin >> configFileInfo;
      fin >> floatInfo;
      storedEngineYaw = floatInfo;
    }
  }
  
  // close file
  fin.close();
}
