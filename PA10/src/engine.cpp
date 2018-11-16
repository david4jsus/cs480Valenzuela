#include "engine.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <string.h>

Engine::Engine(string name, int width, int height, std::string file)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  m_file = file;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
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
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
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
  
  usedRightPaddle = false;
  firstRightPaddleUse = false;
  
  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  // Play imeperial march
  //gameSound.PlayMainSound();

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
      m_graphics->getCamera()->updateCamPosYNeg(m_DT * 0.05);
    }
    else if (movingRight)    // Move camera right
    {
      m_graphics->getCamera()->updateCamPosYPos(m_DT * 0.05);
    }
        
    if (movingForward)       // Move camera forward
    {
      m_graphics->getCamera()->updateCamPosXPos(m_DT * 0.05);
    }
    else if (movingBackward) // Move camera backward
    {
      m_graphics->getCamera()->updateCamPosXNeg(m_DT * 0.05);
    }
        
    if (movingUp)            // Move camera up
    {
      m_graphics->getCamera()->updateCamPosZPos(m_DT * 0.05);
    }
    else if (movingDown)     // Move camera down
    {
      m_graphics->getCamera()->updateCamPosZNeg(m_DT * 0.05);
    }
        
    if (rotatingLeft)        // Rotate camera left
    {
      m_graphics->getCamera()->updateCamRotYaw(m_DT * -0.1);
    }
    else if (rotatingRight)  // Rotate camera right
    {
      m_graphics->getCamera()->updateCamRotYaw(m_DT * 0.1);
    }
    
    if (rotatingUp)          // Rotate camera up
    {
      m_graphics->getCamera()->updateCamRotPitch(m_DT * 0.1);
    }
    else if (rotatingDown)   // Rotate camera down
    {
      m_graphics->getCamera()->updateCamRotPitch(m_DT * -0.1);
    }
    
    // Demo ImGUI window
    //ImGui::ShowDemoWindow(&imgui_demo);
    
    {
      ImGui::Begin("Pinball Instructions and Help");
      
      ImGui::Separator();
      ImGui::Separator();
      
      ImGui::Text("Tab -- Switch between Vertex and Fragment Shaders");
      
      ImGui::Separator();
      ImGui::Separator();
      
      ImGui::Text("Numbad 8 -- Increase Ambient Intesity");
      ImGui::Text("Numbad 2 -- Decrease Ambient Intesity");
      
      ImGui::Separator();
      ImGui::Separator();
      
      ImGui::Text("Numbad 6 -- Increase Specular Intensity");
      ImGui::Text("Numbad 4 -- Decrease Specular Intensity");
      
      ImGui::Separator();
      ImGui::Separator();
      
      int score = m_graphics->getScore();
      std::string scoreText = "Score: ";
      scoreText.append(std::to_string(score));
      const char* displayScoreText = scoreText.c_str();
      ImGui::Text(displayScoreText);
      
      ImGui::Separator();
      ImGui::Separator();
      
      int lives = m_graphics->GetLives();
      std::string liveText = "Lives Remaining: ";
      liveText.append(std::to_string(lives));
      const char* displayLivesRemainingText = liveText.c_str();
      ImGui::Text(displayLivesRemainingText);
      
      /*
      if (ImGui::Button("Use per vertex lighting"))
      {
    	  m_graphics->setVertexShader();
      }
      
      if (ImGui::Button("Use per fragment lighting"))
      {
    	  m_graphics->setFragmentShader();
      }
      
      else if (ImGui::Button("Increase Ambient Lighting"))
      {
        if(m_graphics->getAmbientLightingScale() < 1.0)
        {
          m_graphics->setAmbientLightingScale(m_graphics->getAmbientLightingScale() + 0.1);
        }
      }
      
      else if (ImGui::Button("Decrease Ambient Lighting"))
      {
        if(m_graphics->getAmbientLightingScale() > 0.0)
        {
          m_graphics->setAmbientLightingScale(m_graphics->getAmbientLightingScale() - 0.1);
        }
      }
      
      */
      
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
    
    if (m_event.key.keysym.sym == SDLK_i)   // Rotate camera down
    {
      getObjectRigidBody(1)->applyCentralImpulse(btVector3(0.5f, 0.0f, 0.0f));
    }
    
    if (m_event.key.keysym.sym == SDLK_j)   // Rotate camera down
    {
      getObjectRigidBody(1)->applyCentralImpulse(btVector3(0.0f, 0.0f, -0.5f));
    }
    
    if (m_event.key.keysym.sym == SDLK_k)   // Rotate camera down
    {
      getObjectRigidBody(1)->applyCentralImpulse(btVector3(-0.5f, 0.0f, 0.0f));
    }
    
    if (m_event.key.keysym.sym == SDLK_l)   // Rotate camera down
    {
      getObjectRigidBody(1)->applyCentralImpulse(btVector3(0.0f, 0.0f, 0.5f));
    }
    
    if (m_event.key.keysym.sym == SDLK_TAB) // Switch shaders
    {
    	m_graphics->switchShaders();
    }
    
    if(m_event.key.keysym.sym == SDLK_KP_8)
    {
        if(m_graphics->getAmbientLightingScale() < 1.0)
        {
          m_graphics->setAmbientLightingScale(m_graphics->getAmbientLightingScale() + 0.1);
        }
    }
    
    if(m_event.key.keysym.sym == SDLK_KP_2)
    {
        if(m_graphics->getAmbientLightingScale() > 0.0)
        {
          m_graphics->setAmbientLightingScale(m_graphics->getAmbientLightingScale() - 0.1);
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
    
    startAgainTime = high_resolution_clock::now();
    time_span = startAgainTime - endTime;

    if ((m_event.key.keysym.sym == SDLK_m && usedRightPaddle == false && time_span.count() >= 60.0) || 
        (m_event.key.keysym.sym == SDLK_m && usedRightPaddle == false && firstRightPaddleUse == false))
    {

      //getObjectRigidBody(7)->applyTorqueImpulse(getObjectRigidBody(7)->getWorldTransform().getBasis().getColumn(2) * 1200 * 1000.0f);
      usedRightPaddle = true;
      firstRightPaddleUse = true;
      startTime = high_resolution_clock::now();
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
  int planetCounter;

  if (m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
    // Handle mouse down events here
    if (m_event.button.button == SDL_BUTTON_LEFT) // Reverse direction of rotation of cube
    {
	  //for(planetCounter = 0; planetCounter < m_graphics->numberOfCubes(); planetCounter++)
	  //{
        //m_graphics->GetObject(planetCounter)->reverseDirection();
	  //}
    }
  }
}

unsigned int Engine::getDT()
{
  if(usedRightPaddle == true)
  {
    endTime = high_resolution_clock::now();
    time_span = endTime - startTime;
  
    cout << "Time Span: " << time_span.count() << endl;
  
    if (m_event.key.keysym.sym == SDLK_m && time_span.count() >= 60.0)
    {
      //getObjectRigidBody(7)->applyTorqueImpulse(getObjectRigidBody(7)->getWorldTransform().getBasis().getColumn(2) * 1200 * 1000.0f);

      usedRightPaddle = false;
    }
  }


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

btRigidBody* Engine::getObjectRigidBody(int objectIndex)
{
  return m_graphics->getRigidBody(objectIndex);
}
