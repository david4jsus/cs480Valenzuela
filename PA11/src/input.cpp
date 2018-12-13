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
      case SDLK_e:
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
         break;

      // Shader Toggle (Vertex/Fragment)
      case SDLK_TAB:
         m_graphics->SwitchShaders();
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

      default:
         break;
   }
}

void Input::CheckCameraMovement(unsigned int m_DT)
{
   // Moving Left
   /*if (movingLeft)
   {
      m_graphics->GetCamera()->updateCamPosYNeg(m_DT * 0.05);
   }

   // Moving Right
   else if (movingRight)
   {
      m_graphics->GetCamera()->updateCamPosYPos(m_DT * 0.05);
   }
        
   // Moving Forward
   if (movingForward)
   {
      m_graphics->GetCamera()->updateCamPosXPos(m_DT * 0.05);
   }

   // Moving Backward
   else if (movingBackward)
   {
      m_graphics->GetCamera()->updateCamPosXNeg(m_DT * 0.05);
   }
        
   // Moving Up
   if (movingUp)
   {
      m_graphics->GetCamera()->updateCamPosZPos(m_DT * 0.05);
   }

   // Moving Down
   else if (movingDown)
   {
      m_graphics->GetCamera()->updateCamPosZNeg(m_DT * 0.05);
   }
        
   // Rotate Left
   if (rotatingLeft)
   {
      m_graphics->GetCamera()->updateCamRotYaw(m_DT * -0.1);
   }

   // Rotate Right
   else if (rotatingRight)
   {
      m_graphics->GetCamera()->updateCamRotYaw(m_DT * 0.1);
   }
    
   // Rotate Up
   if (rotatingUp)
   {
      m_graphics->GetCamera()->updateCamRotPitch(m_DT * 0.1);
   }

   // Rotate Down
   else if (rotatingDown)   // Rotate camera down
   {
      m_graphics->GetCamera()->updateCamRotPitch(m_DT * -0.1);
   }*/
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

