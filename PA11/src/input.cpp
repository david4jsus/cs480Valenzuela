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

   return true;
}

void Input::Keyboard(SDL_Event m_event, bool &running)
{
   // Check what event happened
   switch(m_event.type)
   {
      case SDL_KEYDOWN:
cout << "key pressed" << endl;
         KeydownEvents(m_event, running);
         break;

      case SDL_KEYUP:
cout << "key released" << endl;
         KeyupEvents(m_event);
         break;
   }
}

void Input::KeydownEvents(SDL_Event m_event, bool &running)
{
cout << "reached keydown events" << endl;
cout << "event key pressed: " << m_event.key.keysym.sym << endl;
   switch(m_event.key.keysym.sym)
   {
      case SDLK_ESCAPE:
         running = false;
         break;

      // Basic WASDEQ Movement
      // Move Camera Left
      case SDLK_a:
         movingLeft = true;
         movingRight = false;
         break;

      // Move Camera Right
      case SDLK_d:
         movingRight = true; 
         movingLeft = false;
         break;

      // Move Camera Forward
      case SDLK_w:
         movingForward = true;
         movingBackward = false;
         break;

      // Move Camera Backward
      case SDLK_s:
         movingBackward = true;
         movingForward  = false;
         break;

      // Move Camera Up
      case SDLK_e:
         movingUp   = true;
         movingDown = false;
         break;

      // Move Camera Down
      case SDLK_q:
         movingDown = true;
         movingUp   = false;
         break;

      // Basic Camera Rotations
      // Rotate Camera Left
       case SDLK_LEFT:
         rotatingLeft  = true;
         rotatingRight = false;
         break;

      // Rotate Camera Right
      case SDLK_RIGHT:
         rotatingRight = true;
         rotatingLeft  = false;
         break; 

      // Rotate Camera Up
      case SDLK_UP:
         rotatingUp   = true;
         rotatingDown = false;
         break;

      // Rotate Camera Down
      case SDLK_DOWN:
         rotatingDown = true;
         rotatingUp   = false;
         break;

      // Shader Toggle (Vertex/Fragment)
      case SDLK_TAB:
         m_graphics->SwitchShaders();
         break;

      // Increase Ambient Light
      case SDLK_KP_8:
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
         break;

      default:
         break;
   }
}

void Input::KeyupEvents(SDL_Event m_event)
{
   switch(m_event.key.keysym.sym)
   {
      // Basic WASDEQ Movement
      // Move Camera Left
      case SDLK_a:
         movingLeft = false;
         break;

      // Move Camera Right
      case SDLK_d:
         movingRight = false;
         break;

      // Move Camera Forward
      case SDLK_w:
         movingForward = false;
         break;

      // Move Camera Backward
      case SDLK_s:
         movingBackward = false;
         break;

      // Move Camera Up
      case SDLK_e:
         movingUp = false;
         break;

      // Move Camera Down
      case SDLK_q:
         movingDown = false;
         break;

      // Basic Camera Rotations
      // Rotate Camera Left
       case SDLK_LEFT:
         rotatingLeft = false;
         break;

      // Rotate Camera Right
      case SDLK_RIGHT:
         rotatingRight = false;
         break; 

      // Rotate Camera Up
      case SDLK_UP:
         rotatingUp = false;
         break;

      // Rotate Camera Down
      case SDLK_DOWN:
         rotatingDown = false;
         break;

      default:
         break;
   }
}

void Input::CheckCameraMovement(unsigned int m_DT)
{
   // Moving Left
   if (movingLeft)
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
   }
}

void Input::setGraphics(Graphics *engineGraphics)
{
  m_graphics = engineGraphics;
}
