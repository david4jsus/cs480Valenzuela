#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after
  std::string model("");
  if (argc > 1)
  {
    model = argv[1];
  }
  
  Engine *engine = new Engine("CS 480 - Specular Bois Presents - Assimp Model Loader  - PA5", 800, 600, model);
  
  if(!engine->Initialize())
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
