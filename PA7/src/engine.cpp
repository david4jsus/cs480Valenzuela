#include "engine.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

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
    
    // Update position of skybox
    m_graphics->GetObject(0)->setPosition
      (m_graphics->getCamera()->getCamPos());
    
    // Demo ImGUI window
    //ImGui::ShowDemoWindow(&imgui_demo);
    
    {
      ImGui::Begin("Solar System Instructions and Help");
      
      ImGui::Text("Planet Controls");
      if(ImGui::Button("Toggle Direction"))
      {
       for(index = 0; index < m_graphics->numberOfCubes(); index++)
	    {
        m_graphics->GetObject(index)->reverseDirection();
	    }
      }
      
      ImGui::Text("Speed Multipliers");
             
       if (showRotationControls)
       {
          if(ImGui::Button("Hide Rotation Controls"))
          {
             showRotationControls = false;
          }
       }
       else
       {
          if(ImGui::Button("Show Rotation Controls"))
          {
             showRotationControls = true;
          }
       }
      
      if (showRotationControls)
      {
         if(ImGui::Button("0.5x Rotation Speed", ImVec2(200, 50)))
          {
           for(index = 0; index < m_graphics->numberOfCubes(); index++)
           {
            m_graphics->GetObject(index)->UpdateRotationSpeed(0.25f);
           }
          }
          
          else if(ImGui::Button("Normal Rotation Speed", ImVec2(200, 50)))
          {
           for(index = 0; index < m_graphics->numberOfCubes(); index++)
           {
            m_graphics->GetObject(index)->UpdateRotationSpeed(0.5f);
           }
          }
         
          else if(ImGui::Button("1.5x Rotation Speed", ImVec2(200, 50)))
          {
           for(index = 0; index < m_graphics->numberOfCubes(); index++)
           {
            m_graphics->GetObject(index)->UpdateRotationSpeed(1.25f);
           }
          }
         
          else if(ImGui::Button("2.0x Rotation Speed", ImVec2(200, 50)))
          {
           for(index = 0; index < m_graphics->numberOfCubes(); index++)
           {
            m_graphics->GetObject(index)->UpdateRotationSpeed(2.0f);
           }
          }
         
          else if(ImGui::Button("4.0x Rotation Speed", ImVec2(200, 50)))
          {
            // Troll
		        gameSound.LoadSound("../assets/NGGUP.wav");
	          gameSound.PlayNGGUP();
	          
           for(index = 0; index < m_graphics->numberOfCubes(); index++)
           {
            m_graphics->GetObject(index)->UpdateRotationSpeed(4.0f);
           }
          }
       }
       
      if (showOrbitControls)
      {
         if(ImGui::Button("Hide Orbit Controls"))
         {
            showOrbitControls = false;
         }
      }
      else
      {
         if(ImGui::Button("Show Orbit Controls"))
         {
            showOrbitControls = true;
         }
      }
      
      if (showOrbitControls)
      {
       if(ImGui::Button("0.5x Orbit Speed", ImVec2(200, 50)))
       {
        for(index = 0; index < m_graphics->numberOfCubes(); index++)
        {
         m_graphics->GetObject(index)->UpdateOrbitSpeed(0.25f);
        }
       }
       
       else if(ImGui::Button("Normal Orbit Speed", ImVec2(200, 50)))
       {
        for(index = 0; index < m_graphics->numberOfCubes(); index++)
        {
         m_graphics->GetObject(index)->UpdateOrbitSpeed(0.5f);
        }
       }
      
       else if(ImGui::Button("1.5x Orbit Speed", ImVec2(200, 50)))
       {
        for(index = 0; index < m_graphics->numberOfCubes(); index++)
        {
         m_graphics->GetObject(index)->UpdateOrbitSpeed(1.25f);
        }
       }
      
       else if(ImGui::Button("2.0x Orbit Speed", ImVec2(200, 50)))
       {
        for(index = 0; index < m_graphics->numberOfCubes(); index++)
        {
         m_graphics->GetObject(index)->UpdateOrbitSpeed(2.0f);
        }
       }
      
       else if(ImGui::Button("4.0x Orbit Speed", ImVec2(200, 50)))
       {
        for(index = 0; index < m_graphics->numberOfCubes(); index++)
        {
         m_graphics->GetObject(index)->UpdateOrbitSpeed(4.0f);
        }
       }
      }
      
      ImGui::Text("Teleportation Controls");
      
      if (showTeleportControls)
      {
         if(ImGui::Button("Hide Teleportation Controls"))
         {
            showTeleportControls = false;
         }
      }
      else
      {
         if(ImGui::Button("Show Teleport Controls"))
         {
            showTeleportControls = true;
         }
      }
      
      if (showTeleportControls)
      {
      
         ImGui::Text("Click on one of the following astal objects to teleport to it");
         for (int i = 1; i < 22; i++)
         {
            if (ImGui::Button(m_graphics->GetObject(i)->GetObjectName().c_str()))
            {
               m_graphics->getCamera()->setCamPos
                  (m_graphics->GetObject(i)->objectPosition());
            }
         }
      }
      
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
