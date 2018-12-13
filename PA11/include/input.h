#ifndef INPUT_HEADER
#define INPUT_HEADER
#include "graphics.h"
#include "graphics_headers.h"
#include "window.h"
#include <assert.h>
#include <sys/time.h>
#include <iostream>
#include <btBulletDynamicsCommon.h>

using namespace std;

class Input {

   public:

      Input();
      ~Input();

      bool Initialize();

      // Keyboard Inputs
      void Keyboard(SDL_Event m_event, bool &running);
      void KeydownEvents(SDL_Event m_event, bool &running);
      void KeyupEvents(SDL_Event m_event);
      void CheckCameraMovement(unsigned int m_DT);
      
      void setGraphics(Graphics *engineGraphics);

			void CheckPlayerMovement();

			btRigidBody* GetObjectRigidBody(string objectName);

   private:
      Graphics *m_graphics;

		  // Player one movement handling
		  bool playerOneMoveForward;
		  bool playerOneMoveBackward;
		  bool playerOneMoveLeft;
		  bool playerOneMoveRight;
		  
		  // Player one movement handling
		  bool playerTwoMoveForward;
		  bool playerTwoMoveBackward;
		  bool playerTwoMoveLeft;
			bool playerTwoMoveRight;
};

#endif
