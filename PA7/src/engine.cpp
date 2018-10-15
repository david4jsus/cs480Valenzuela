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

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

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
    
    // Demo ImGUI window
    //ImGui::ShowDemoWindow(&imgui_demo);
    
    {
      ImGui::Begin("Solar System Instructions and Help");
      
      ImGui::Text("Camera Controls");
      
      ImGui::Text("Planet Controls");
      if(ImGui::Button("Toggle Direction"))
      {
       for(int index = 0; index < m_graphics->numberOfCubes(); index++)
	    {
        m_graphics->GetObject(index)->reverseDirection();
	    }
      }
      
      ImGui::Text("Speed Multiplier");
           
      if(ImGui::Button("Normal Speed", ImVec2(100, 50)))
      {
       m_graphics->GetObject(0)->UpdateSpeed(0.1f);
      }
      
      else if(ImGui::Button("1.5x Speed", ImVec2(100, 50)))
      {
       m_graphics->GetObject(0)->UpdateSpeed(0.2f);
      }
      
      else if(ImGui::Button("2.0x Speed", ImVec2(100, 50)))
      {
       m_graphics->GetObject(0)->UpdateSpeed(0.4f);
      }
      
      else if(ImGui::Button("4.0x Speed", ImVec2(100, 50)))
      {
       m_graphics->GetObject(0)->UpdateSpeed(0.8f);
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
  int planetCounter;
   
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
    
    if (m_event.key.keysym.sym == SDLK_a)      // Reverse direction of rotation of cube
    {
	  for(planetCounter = 0; planetCounter < m_graphics->numberOfCubes(); planetCounter++)
	  {
        m_graphics->GetObject(planetCounter)->reverseDirection();
	  }
    }
    
    if (m_event.key.keysym.sym == SDLK_LEFT)      // Make direction of rotation of cube counter-clockwise
    {
	  for(planetCounter = 0; planetCounter < m_graphics->numberOfCubes(); planetCounter++)
	  {
        m_graphics->GetObject(planetCounter)->makeDirectionCounter();
	  }
    }
    
    if (m_event.key.keysym.sym == SDLK_RIGHT)      // Make direction of rotation of cube clockwise
    {
	  for(planetCounter = 0; planetCounter < m_graphics->numberOfCubes(); planetCounter++)
	  {
        m_graphics->GetObject(planetCounter)->makeDirectionClockwise();
	  }
    }
    
    if (m_event.key.keysym.sym == SDLK_s)      // Stop cube orbit
    {
	  for(planetCounter = 0; planetCounter < m_graphics->numberOfCubes(); planetCounter++)
	  {
        m_graphics->GetObject(planetCounter)->toggleOrbit();
	  }
    }
    
    if (m_event.key.keysym.sym == SDLK_d)      // Stop cube rotation
    {
	  for(planetCounter = 0; planetCounter < m_graphics->numberOfCubes(); planetCounter++)
	  {
        m_graphics->GetObject(planetCounter)->toggleRotation();
	  }
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
