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
  
  // create player settings
  players = new PlayerSettings();

  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics(storedVLightingVertexShaderFilePath, storedVLightingFragmentShaderFilePath, storedFLightingVertexShaderFilePath, storedFLightingFragmentShaderFilePath, 
                            storedEngineStartingCameraPos, storedEngineYaw, storedEnginePitch, gravityDirection, jumpHeight, allObjectsInfo);
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

	// set player settings
	setPlayerSettings();

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();
  
  // Setup Dear ImGui binding
  ImGui::CreateContext();
  ImGuiIO& imgui_io = ImGui::GetIO(); (void)imgui_io;
  ImGui_ImplSDL2_InitForOpenGL(m_window->getSDLWindow(), m_window->getGLContext());
  ImGui_ImplOpenGL3_Init("#version 130"); // GL 3.0 + GLSL 130
  ImGui::StyleColorsDark(); // Setup style
  
  // Stadium Sound
  gameSound.LoadSound("../assets/sounds/ps1-ssbm.wav");
  gameSound.PlaySound();
  
  soundPlayed = false;
  
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
      m_input->Mouse(m_event);
    }
    
    m_input->CheckCamera(m_DT);
	 m_input->CheckPlayerMovement();
    
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

			// display player 1's remaining lives
      int playerOneLives, playerTwoLives;
			players->getPlayersLives(playerOneLives, playerTwoLives);
      std::string playerOneLivesText = "Player 1 lives remaining: ";
      playerOneLivesText.append(std::to_string(playerOneLives));
      const char* displayPlayerOneLivesText = playerOneLivesText.c_str();
      ImGui::Text(displayPlayerOneLivesText);
      
      ImGui::Separator();
      ImGui::Separator();

			// display player 2's remaining lives
			std::string playerTwoLivesText = "Player 2 lives remaining: ";
      playerTwoLivesText.append(std::to_string(playerTwoLives));
      const char* displayPlayerTwoLivesText = playerTwoLivesText.c_str();
      ImGui::Text(displayPlayerTwoLivesText);

      ImGui::Separator();
      ImGui::Separator();

			if(playerOneLives <= 0)
			{
				ImGui::Text("Player 2 wins!");
				
				if(players->GameOver() && players->GameRestart() && !soundPlayed)
				{			  
				  soundPlayed = true;	
				  gameSound.LoadSound("../assets/sounds/win.wav");
          gameSound.PlaySound();
				  gameSound.LoadSound("../assets/sounds/p1.wav");
          gameSound.PlaySound();
          gameSound.LoadSound("../assets/sounds/defeated.wav");
          gameSound.PlaySound();
         }
			}

			else if(playerTwoLives <= 0)
			{
				ImGui::Text("Player 1 wins!");

				
				if(players->GameOver() && players->GameRestart() && !soundPlayed)
				{			
				  soundPlayed = true;
				  gameSound.LoadSound("../assets/sounds/win.wav");
          gameSound.PlaySound();
				  gameSound.LoadSound("../assets/sounds/p2.wav");
          gameSound.PlaySound();
          gameSound.LoadSound("../assets/sounds/defeated.wav");
          gameSound.PlaySound();
         }
			}
			
			  ImGui::Separator();
        ImGui::Separator();

			// display restart game button when one player dies
			if(playerOneLives <= 0 || playerTwoLives <= 0)
			{
				if(ImGui::Button("Restart Game"))
				{			   				
		      // restart game
					restartGame();
				}
			}    

    	ImGui::Separator();
      ImGui::Separator();

			if(ImGui::Button("Increase ambient lighting"))
			{
				if(m_graphics->GetAmbientLightingScale() < 1.0)
				{
					m_graphics->SetAmbientLightingScale(m_graphics->GetAmbientLightingScale() + 0.1);
				}
			}

			if(ImGui::Button("Decrease ambient lighting"))
			{
				if(m_graphics->GetAmbientLightingScale() > 0.0)
				{
					m_graphics->SetAmbientLightingScale(m_graphics->GetAmbientLightingScale() - 0.1);
				}
			}

			if(ImGui::Button("Increase specular lighting"))
			{
				if(m_graphics->GetAmbientLightingScale() < 1.0)
				{
					m_graphics->SetSpecularScale(m_graphics->GetSpecularScale() + 0.1);
				}
			}

			if(ImGui::Button("Decrease specular lighting"))
			{
				if(m_graphics->GetAmbientLightingScale() > 0.0)
				{
					m_graphics->SetSpecularScale(m_graphics->GetSpecularScale() - 0.1);
				}
			}

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

    // check if we are about to read in gravity direction information
    else if(configFileInfo == "gravity:")
    {
      // get gravity x direction
      fin >> xAxisInfo;
      fin >> configFileInfo;
      
      // get gravity y direction
      fin >> yAxisInfo;
      fin >> configFileInfo;
      
      // get gravity z direction
      fin >> zAxisInfo;

			// set gravity direction
			gravityDirection = btVector3(xAxisInfo, yAxisInfo, zAxisInfo);
    }

    // check if we are about to read in jump height information
    else if(configFileInfo == "power:")
    {
			// get jump power information
			fin >> jumpHeight;
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

    // check if we are about to read in object bounciness (restitution) information
    else if(configFileInfo == "restitution:")
    {
      // get object mass
      fin >> anObject.restitution;
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
      }
      
      // get sphere collision size
      else if(anObject.collisionShapeType == "sphere")
      {
        // get radius size for sphere
        fin >> anObject.sphereRadius;
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
      }

			// get capsule collision size
			else if(anObject.collisionShapeType == "capsule")
			{
				// get capsule raidus
				fin >> anObject.capsuleRadius;
				fin >> configFileInfo;

				// get capsule height
				fin >> anObject.capsuleHeight;
			}

			// get cylinder collision size
			else if(anObject.collisionShapeType == "cylinder")
			{
       // get collision shape x length
        fin >> xAxisInfo;
        fin >> configFileInfo;
        
        // get collision shape y length
        fin >> yAxisInfo;
        fin >> configFileInfo;
        
        // get collision shape z length
        fin >> zAxisInfo;
        
        // set box capsule size
        anObject.cylinderSize = btVector3(xAxisInfo, yAxisInfo, zAxisInfo);
			}
      
      // store object onto list of objects
      allObjectsInfo.push_back(anObject);
    }
  }
  
  // close file
  fin.close();
}

void Engine::setPlayerSettings()
{
  m_graphics->setPlayerSettings(players);
}

void Engine::restartGame()
{
	// local variables
	int playerOneLives, playerTwoLives;
   
	// reset players lives
	playerOneLives = 3;
	playerTwoLives = 3;
	players->setPlayersLives(playerOneLives, playerTwoLives);

	// reset players position
	GetObjectRigidBody("Player1")->setCenterOfMassTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(10, 10, 0)));
	GetObjectRigidBody("Player2")->setCenterOfMassTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-10, 10, 0)));
}
