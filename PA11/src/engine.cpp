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
  
  // Player one movement handling
  playerOneMoveForward = false;
  playerOneMoveBackward = false;
  playerOneMoveLeft = false;
  playerOneMoveRight = false;
  
  // Player one movement handling
  playerTwoMoveForward = false;
  playerTwoMoveBackward = false;
  playerTwoMoveLeft = false;
  playerTwoMoveRight = false;
  
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
    
    /*/////////////////////////////////////////////////////////////////////
                            Camera movement
    /////////////////////////////////////////////////////////////////////*/
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
    
    /*/////////////////////////////////////////////////////////////////////
                            Player one movement
    /////////////////////////////////////////////////////////////////////*/
    if (playerOneMoveForward) // Move player one forward
    {
      GetObjectRigidBody("Player1")->applyCentralImpulse(btVector3(0.3f, 0.0f, 0.0f));
    }
    
    else if (playerOneMoveBackward) // Move player one backward
    {
      GetObjectRigidBody("Player1")->applyCentralImpulse(btVector3(-0.3f, 0.0f, 0.0f));
    }
    
    if (playerOneMoveLeft)  // Move player one left
    {
      GetObjectRigidBody("Player1")->applyCentralImpulse(btVector3(0.0f, 0.0f, -0.3f));
    }
    
    else if (playerOneMoveRight) // Move player one right
    {
      GetObjectRigidBody("Player1")->applyCentralImpulse(btVector3(0.0f, 0.0f, 0.3f));
    }
    
    /*/////////////////////////////////////////////////////////////////////
                            Player two movement
    /////////////////////////////////////////////////////////////////////*/
    if (playerTwoMoveForward) // Move player two forward
    {
      GetObjectRigidBody("Player2")->applyCentralImpulse(btVector3(0.3f, 0.0f, 0.0f));
    }
    
    else if (playerTwoMoveBackward) // Move player two backward
    {
      GetObjectRigidBody("Player2")->applyCentralImpulse(btVector3(-0.3f, 0.0f, 0.0f));
    }
    
    if (playerTwoMoveLeft) // Move player two left
    {
      GetObjectRigidBody("Player2")->applyCentralImpulse(btVector3(0.0f, 0.0f, -0.3f));
    }
    
    else if (playerTwoMoveRight)  // Move player two right
    {
      GetObjectRigidBody("Player2")->applyCentralImpulse(btVector3(0.0f, 0.0f, 0.3f));
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
    
    if (m_event.key.keysym.sym == SDLK_i)   // Move player one forward
    {
      playerOneMoveForward = true;
      playerOneMoveBackward = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_j)   // Move player one left
    {
      playerOneMoveLeft = true;
      playerOneMoveRight = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_k)   // Move player one backward
    {
      playerOneMoveBackward = true;
      playerOneMoveForward = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_l)   // Move player one right
    {
      playerOneMoveRight = true;
      playerOneMoveLeft = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_t)   // Move player two forward
    {
      playerTwoMoveForward = true;
      playerTwoMoveBackward = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_f)   // Move player two left
    {
      playerTwoMoveLeft = true;
      playerTwoMoveRight = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_g)   // Move player two backward
    {
      playerTwoMoveBackward = true;
      playerTwoMoveForward = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_h)   // Move player two right
    {
      playerTwoMoveRight = true;
      playerTwoMoveLeft = false;
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
    
    
    
    
    if (m_event.key.keysym.sym == SDLK_i)   // Move player 1 ball forward
    {
      playerOneMoveForward = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_j)   // Move player 1 ball to the left
    {
      playerOneMoveLeft = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_k)   // Move player 1 ball backward
    {
      playerOneMoveBackward = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_l)   // Move player 1 ball to the right
    {
      playerOneMoveRight = false;
    }
    
    
    
    
    
    if (m_event.key.keysym.sym == SDLK_t)   // Move player 2 ball forward
    {
      playerTwoMoveForward = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_f)   // Move player 2 ball to the left
    {
      playerTwoMoveLeft = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_g)   // Move player 2 ball backward
    {
      playerTwoMoveBackward = false;
    }
    
    if (m_event.key.keysym.sym == SDLK_h)   // Move player 2 ball to the right
    {
      playerTwoMoveRight = false;
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

btRigidBody* Engine::GetObjectRigidBody(string objectName)
{
  return m_graphics->GetRigidBody(objectName);
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
    
    // check if entire file has been read
    if(fin.eof() == true)
    {
      break;
    }
    
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
      fin >> anObject.collisionShapeType;
    }
    
    // check if we are about to read in object collision shape size information
    else if(configFileInfo == "size:")
    {
      // get sphere collision size
      if(anObject.collisionShapeType == "box")
      {
        // get collision shape x length
        fin >> xAxisInfo;
        fin >> configFileInfo;
        
        // get collision shape y length
        fin >> yAxisInfo;
        fin >> configFileInfo;
        
        // get collision shape z length
        fin >> zAxisInfo;
        
        // set box collision size
        anObject.boxSize = btVector3(xAxisInfo, yAxisInfo, zAxisInfo);
        
        // default other values
        anObject.sphereRadius = 0.0;
        anObject.planeDirection = btVector3(0.0, 0.0, 0.0);
        anObject.planeConstant = 0.0;
      }
      
      // get sphere collision size
      else if(anObject.collisionShapeType == "sphere")
      {
        // get radius size for sphere
        fin >> anObject.sphereRadius;
        
        // default other values
        anObject.boxSize = btVector3(0.0, 0.0, 0.0);
        anObject.planeDirection = btVector3(0.0, 0.0, 0.0);
        anObject.planeConstant = 0.0;
      }
      
      // get plane collision direction
      else if(anObject.collisionShapeType == "plane")
      {
        // get plane x-axis direction
        fin >> xAxisInfo;
        fin >> configFileInfo;
        
        // get plane y-axis direction
        fin >> yAxisInfo;
        fin >> configFileInfo;
        
        // get plane z-axis direction
        fin >> zAxisInfo;
        fin >> configFileInfo;
        
        // get plane constant
        fin >> anObject.planeConstant;
        
        // set plane collision direction
        anObject.planeDirection = btVector3(xAxisInfo, yAxisInfo, zAxisInfo);
        
        // default other values
        anObject.boxSize = btVector3(0.0, 0.0, 0.0);
        anObject.sphereRadius = 0.0;
      }
      
      // store object onto list of objects
      allObjectsInfo.push_back(anObject);
    }
  }
  
  // close file
  fin.close();
}
