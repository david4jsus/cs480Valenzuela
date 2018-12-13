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
      
      // Mouse Inputs
      void Mouse(SDL_Event m_event);
      
      void CheckCamera(unsigned int m_DT);
      
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
	  
	  // Mouse clicking + movement
	  bool leftClicking;
	  float deltax;
	  float deltay;
	  
	  // Keyboard input
	  bool shifting;
};

#endif
