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
                            storedEngineStartingCameraPos, storedEngineYaw, storedEnginePitch, allObjectsInfo);
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, m_file))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }
  
  // Start the input
  m_input = new Input();
  if(!m_input->Initialize())
  {
     printf("The input failed to initialize.\n");
     return false;
  }
  m_input->setGraphics(m_graphics);

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();
  
  // Setup Dear ImGui binding
  ImGui::CreateContext();
  ImGuiIO& imgui_io = ImGui::GetIO(); (void)imgui_io;
  ImGui_ImplSDL2_InitForOpenGL(m_window->getSDLWindow(), m_window->getGLContext());
  ImGui_ImplOpenGL3_Init("#version 130"); // GL 3.0 + GLSL 130
  ImGui::StyleColorsDark(); // Setup style
  
  /*
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
  */
  
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
      m_input->Keyboard(m_event, m_running);
    }
    
    m_input->CheckCameraMovement(m_DT);
    
    /////////////////////////////////////////////////
    /////////////// IMGUI MENU SYSTEM ///////////////
    /////////////////////////////////////////////////
    {
      ImGui::Begin("Joust Instructions and Help");
      
      ImGui::Separator();
      ImGui::Separator();
      
      ImGui::Text("Tab -- Switch between Vertex and Fragment Shaders");
      
      ImGui::Separator();
      ImGui::Separator();
      
      ImGui::End();
    }
    /////////////////////////////////////////////////
    /////////////////////////////////////////////////
    
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

unsigned int Engine::getDT()
{
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
  ObjectInfo anObject;
  float xAxisInfo, yAxisInfo, zAxisInfo, aAxisInfo;
  
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
      storedEnginePitch = floatInfo;
    }
    
    // check if we are about to read in object name information
    else if(configFileInfo == "name:")
    {
      // get object name
      fin >> anObject.objectName;
    }
    
    // check if we are about to read in object position information
    else if(configFileInfo == "position:")
    {
      // get object x position
      fin >> xAxisInfo;
      fin >> configFileInfo;
      
      // get object y position
      fin >> yAxisInfo;
      fin >> configFileInfo;
      
      // get object z position
      fin >> zAxisInfo;
      
      // set object position
      anObject.objectPos = btVector3(xAxisInfo, yAxisInfo, zAxisInfo);
    }
    
    // check if we are about to read in object orientation information
    else if(configFileInfo == "orientation:")
    {
      // get object x position
      fin >> xAxisInfo;
      fin >> configFileInfo;
      
      // get object y position
      fin >> yAxisInfo;
      fin >> configFileInfo;
      
      // get object z position
      fin >> zAxisInfo;
      fin >> configFileInfo;
      
      // get fourth number required for orientation
      fin >> aAxisInfo;
      
      // set object orientation
      anObject.objectOrientation = btQuaternion(xAxisInfo, yAxisInfo, zAxisInfo, aAxisInfo);
    }
    
    // check if we are about to read in object scale information
    else if(configFileInfo == "scale:")
    {
      // get object scaling size
      fin >> anObject.scale;
    }
    
    // check if we are about to read in object mass information
    else if(configFileInfo == "mass:")
    {
      // get object mass
      fin >> anObject.mass;
    }
    
    // check if we are about to read in object collision shape information
    else if(configFileInfo == "shape:")
    {
      // get collision shape name
      fin >> anObject.collisionShape;
    }
    
    // check if we are about to read in object collision shape size information
    else if(configFileInfo == "size:")
    {
      // get sphere collision size
      if(anObject.collisionShape == "box")
      {
        // get collision shape x length
        fin >> anObject.xBoxSize;
        fin >> configFileInfo;
        
        // get collision shape y length
        fin >> anObject.yBoxSize;
        fin >> configFileInfo;
        
        // get collision shape z length
        fin >> anObject.zBoxSize;
        
        // default other values
        anObject.sphereRadius = 0.0;
        anObject.planeDirection = 0.0;
        anObject.planeConstant = 0.0;
      }
      
      // get sphere collision size
      else if(anObject.collisionShape == "sphere")
      {
        // get radius size
        fin >> anObject.sphereRadius;
        
        // default other values
        anObject.xBoxSize = 0.0;
        anObject.yBoxSize = 0.0;
        anObject.zBoxSize = 0.0;
        anObject.planeDirection = 0.0;
        anObject.planeConstant = 0.0;
      }
      
      // get plane collision direction
      else if(anObject.collisionShape == "plane")
      {
        // get plane direction
        fin >> anObject.planeDirection;
        fin >> anObject.planeConstant;
        
        // default other values
        anObject.xBoxSize = 0.0;
        anObject.yBoxSize = 0.0;
        anObject.zBoxSize = 0.0;
        anObject.sphereRadius = 0.0;
      }
      
      // store object onto list of objects
      allObjectsInfo.push_back(anObject);
    }
  }
  
  // close file
  fin.close();
}
