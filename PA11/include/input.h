#ifndef INPUT_HEADER
#define INPUT_HEADER
#include "input.h"
#include "graphics.h"
#include "graphics_headers.h"
#include "window.h"
#include <assert.h>
#include <sys/time.h>
#include <iostream>

using namespace std;

class Input {

   public:

      Input();
      ~Input();

      bool Initialize();

      // Keyboard Inputs
      void Keyboard(SDL_Event m_event, bool running);
      void KeydownEvents(SDL_Event m_event, bool running);
      void KeyupEvents(SDL_Event m_event);
      void CheckCameraMovement(unsigned int m_DT);

   private:
      Graphics *m_graphics;

      // Keyboard handling
      bool movingRight;
      bool movingLeft;
      bool movingForward;
      bool movingBackward;
      bool movingUp;
      bool movingDown;
      bool rotatingLeft;
      bool rotatingRight;
      bool rotatingUp;
      bool rotatingDown;
};

#endif