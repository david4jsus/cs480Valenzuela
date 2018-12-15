#include "input.h"

Input::Input()
{
   
}

Input::~Input()
{
   m_graphics = NULL;
}

bool Input::Initialize()
{
   playerOneMoveForward = false;
   playerOneMoveBackward = false;
   playerOneMoveLeft = false;
   playerOneMoveRight = false;

   playerTwoMoveForward = false;
   playerTwoMoveBackward = false;
   playerTwoMoveLeft = false;
   playerTwoMoveRight = false;
   
   leftClicking = false;
   deltax = 0;
   deltay = 0;
   
   shifting = false;

	 startTimePlayerOneJump = high_resolution_clock::now();
	 endTimePlayerOneJump = high_resolution_clock::now();
	 startTimePlayerTwoJump = high_resolution_clock::now();
	 endTimePlayerTwoJump = high_resolution_clock::now();

   return true;
}

void Input::Keyboard(SDL_Event m_event, bool &running)
{
   // Check what event happened
   switch(m_event.type)
   {
      case SDL_KEYDOWN:
         KeydownEvents(m_event, running);
         break;

      case SDL_KEYUP:
         KeyupEvents(m_event);
         break;
   }
}

void Input::KeydownEvents(SDL_Event m_event, bool &running)
{
   switch(m_event.key.keysym.sym)
   {
      case SDLK_ESCAPE:
         running = false;
         break;

      // Basic WASDEQ Movement
      // Move Camera Left
      case SDLK_w:
		     playerOneMoveForward = true;
		     playerOneMoveBackward = false;
         break;

      // Move Camera Right
      case SDLK_a:
		     playerOneMoveLeft = true;
		     playerOneMoveRight = false;
         break;

      // Move Camera Forward
      case SDLK_s:
		     playerOneMoveBackward = true;
		     playerOneMoveForward = false;
         break;

      // Move Camera Backward
      case SDLK_d:
		     playerOneMoveRight = true;
		     playerOneMoveLeft = false;
         break;

      // Move Camera Up
      case SDLK_SPACE:
				 playerOneJump();
         break;

			// Move player two forward
			case SDLK_KP_8:
		     playerTwoMoveForward = true;
		     playerTwoMoveBackward = false;
         break;

			// Move player two left
			case SDLK_KP_4:
		     playerTwoMoveLeft = true;
		     playerTwoMoveRight = false;
         break;

			// Move player two backward
			case SDLK_KP_2:
		     playerTwoMoveBackward = true;
		     playerTwoMoveForward = false;
         break;

			// Move player two right
			case SDLK_KP_6:
		     playerTwoMoveRight = true;
		     playerTwoMoveLeft = false;
         break;

			// player two jump
			case SDLK_KP_0: 
				playerTwoJump();
         break;

      // Shader Toggle (Vertex/Fragment)
      case SDLK_TAB:
         m_graphics->SwitchShaders();
         break;
         
      case SDLK_LSHIFT:
         shifting = true;
         break;
         
      case SDLK_RSHIFT:
         shifting = true;
         break;

      // Increase Ambient Light
      /*case SDLK_KP_8:
         if(m_graphics->GetAmbientLightingScale() < 1.0)
         {
            m_graphics->SetAmbientLightingScale(m_graphics->GetAmbientLightingScale() + 0.1);
         }
         break;

      // Decrease Ambient Light
      case SDLK_KP_2:
         if(m_graphics->GetAmbientLightingScale() > 0.0)
         {
            m_graphics->SetAmbientLightingScale(m_graphics->GetAmbientLightingScale() - 0.1);
         }
         break;

      // Increase Specular Light
      case SDLK_KP_6:
         if(m_graphics->GetSpecularScale() < 1.0)
         {
            m_graphics->SetSpecularScale(m_graphics->GetSpecularScale() + 0.1);
         }
         break;

      // Decrease Specular Light
      case SDLK_KP_4:
         if(m_graphics->GetSpecularScale() > 0.0)
         {
            m_graphics->SetSpecularScale(m_graphics->GetSpecularScale() - 0.1);
         }
         break; */

      default:
         break;
   }
}

void Input::KeyupEvents(SDL_Event m_event)
{
   switch(m_event.key.keysym.sym)
   {
			// stop moving player one forward
			case SDLK_w:
				playerOneMoveForward = false;
				break;

			case SDLK_a:
				playerOneMoveLeft = false;
				break;

			case SDLK_s:
				playerOneMoveBackward = false;
				break;

			case SDLK_d:
				playerOneMoveRight = false;
				break;

			case SDLK_KP_8:
				playerTwoMoveForward = false;
				break;

			case SDLK_KP_4:
				playerTwoMoveLeft = false;
				break;

			case SDLK_KP_2:
				playerTwoMoveBackward = false;
				break;

			case SDLK_KP_6:
				playerTwoMoveRight = false;
				break;
				
		      case SDLK_LSHIFT:
		         shifting = false;
		         break;
		         
		      case SDLK_RSHIFT:
		         shifting = false;
		         break;

      default:
         break;
   }
}

void Input::Mouse(SDL_Event m_event)
{
	// Get mouse movement
	if (m_event.type == SDL_MOUSEMOTION)
	{
		deltax = m_event.motion.xrel;
		deltay = m_event.motion.yrel;
	}
	else
	{
		deltax = 0;
		deltay = 0;
	}
	
	// Get left click
	if (m_event.button.button == SDL_BUTTON_LEFT)
	{
		leftClicking = true;
	}
	else
	{
		leftClicking = false;
	}
}

void Input::CheckCamera(unsigned int m_DT)
{
	if (leftClicking) // Engage camera stuffs!
	{
		if (!shifting) // Camera rotation
		{
			m_graphics->GetCamera()->updateCamRotYaw(m_DT * deltax / 50);
			m_graphics->GetCamera()->updateCamRotPitch(m_DT * deltay / 50);
		}
		else // Camera translation
		{
			if (deltax > 0)
			{
				m_graphics->GetCamera()->updateCamPosYPos(m_DT * 0.05);
			}
			else if (deltax < 0)
			{
				m_graphics->GetCamera()->updateCamPosYNeg(m_DT * 0.05);
			}
			
			if (deltay > 0)
			{
				m_graphics->GetCamera()->updateCamPosXPos(m_DT * 0.1);
			}
			else if (deltay < 0)
			{
				m_graphics->GetCamera()->updateCamPosXNeg(m_DT * 0.1);
			}
		}
	}
}

void Input::setGraphics(Graphics *engineGraphics)
{
  m_graphics = engineGraphics;
}

void Input::CheckPlayerMovement()
{
  /*/////////////////////////////////////////////////////////////////////
                          Player one movement
  /////////////////////////////////////////////////////////////////////*/
	if (playerOneMoveForward) // Move player one forward
  {
    GetObjectRigidBody("Player1")->applyCentralImpulse(btVector3(0.05f, 0.0f, 0.0f));
  }
    
  else if (playerOneMoveBackward) // Move player one backward
  {
    GetObjectRigidBody("Player1")->applyCentralImpulse(btVector3(-0.05f, 0.0f, 0.0f));
  }
    
  if (playerOneMoveLeft)  // Move player one left
  {
    GetObjectRigidBody("Player1")->applyCentralImpulse(btVector3(0.0f, 0.0f, -0.05f));
  }
    
  else if (playerOneMoveRight) // Move player one right
  {
    GetObjectRigidBody("Player1")->applyCentralImpulse(btVector3(0.0f, 0.0f, 0.05f));
  }
    
  /*/////////////////////////////////////////////////////////////////////
                          Player two movement
  /////////////////////////////////////////////////////////////////////*/
  if (playerTwoMoveForward) // Move player two forward
  {
    GetObjectRigidBody("Player2")->applyCentralImpulse(btVector3(0.05f, 0.0f, 0.0f));
  }
    
  else if (playerTwoMoveBackward) // Move player two backward
  {
    GetObjectRigidBody("Player2")->applyCentralImpulse(btVector3(-0.05f, 0.0f, 0.0f));
  }
    
  if (playerTwoMoveLeft) // Move player two left
  {
    GetObjectRigidBody("Player2")->applyCentralImpulse(btVector3(0.0f, 0.0f, -0.05f));
  }
    
  else if (playerTwoMoveRight)  // Move player two right
  {
    GetObjectRigidBody("Player2")->applyCentralImpulse(btVector3(0.0f, 0.0f, 0.05f));
  }
}

btRigidBody* Input::GetObjectRigidBody(string objectName)
{
	return m_graphics->GetRigidBody(objectName);
}

void Input::playerOneJump()
{
	// calculate player 1 jump period
	endTimePlayerOneJump = high_resolution_clock::now();
	time_span = endTimePlayerOneJump - startTimePlayerOneJump;


	if(time_span.count() >= 300)
	{
		GetObjectRigidBody("Player1")->applyCentralImpulse(btVector3(0.0f, m_graphics->getJumpHeight(), 0.0f));
		startTimePlayerOneJump = high_resolution_clock::now();
	}
}

void Input::playerTwoJump()
{
	// calculate player 1 jump period
	endTimePlayerTwoJump = high_resolution_clock::now();
	time_span = endTimePlayerTwoJump - startTimePlayerTwoJump;


	if(time_span.count() >= 300)
	{
		GetObjectRigidBody("Player2")->applyCentralImpulse(btVector3(0.0f, m_graphics->getJumpHeight(), 0.0f));
		startTimePlayerTwoJump = high_resolution_clock::now();
	}
}
