#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height, std::string file)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Create objects
  Object* sun = new Object("Sun.obj", 0, 0.0f, 0.0f, 0.1f, 0.04f); // file path, parent, orbit radius size, orbit radius speed, local rotation speed, object size
  Object* mercury = new Object("Mercury.obj", 0, 0.0f, 0.0f, 0.1f, 0.07f);
  Object* venus  = new Object("Venus.obj", 0, 0.0f, 0.0f, 0.1f, 0.07f);
  Object* earth = new Object("Earth.obj", 0, 0.0f, 0.0f, 0.1f, 0.07f);
  Object* earthMoon = new Object("Moon.obj", earth, 0.0f, 0.0f, 0.1f, 0.07f);
  Object* mars   = new Object("Mars.obj", 0, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* marsMoonPhobos = new Object("Phobos.obj", mars, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* marsMoonDeimos = new Object("Moon.obj", mars, 0.0f, 0.0f, 0.1f, 0.07f);
  Object* jupiter = new Object("Jupiter.obj", 0, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* jupiterMoonGanymede = new Object("Ganymede.obj", jupiter, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* jupiterCallisto = new Object("Callisto.obj", jupiter, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* jupiterMoonIo = new Object("Io.obj", jupiter, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* jupiterMoonEuropa = new Object("Europa.obj", jupiter, 0.0f, 0.0f, 0.1f, 0.07f);
  Object* saturnRings = new Object("Saturn2.obj", 0, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* saturnMoonTitan = new Object("Titan.obj", saturnRings, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* saturnMoonRhea = new Object("Rhea.obj", saturnRings, 0.0f, 0.0f, 0.1f, 0.07f);
  Object* uranus = new Object("Uranus.obj", 0, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* uranusMoonOberon = new Object("Oberon.obj", uranus, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* uranusMoonUmbriel = new Object("Umbriel.obj", uranus, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* uranusMoonAriel = new Object("Ariel.obj", uranus, 0.0f, 0.0f, 0.1f, 0.07f);
  Object* neptune = new Object("Neptune.obj", 0, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* neptuneMoonTriton = new Object("Triton.obj", neptune, 0.0f, 0.0f, 0.1f, 0.07f);
  //Object* neptuneMoonProteus = new Object("Proteus.obj", neptune, 0.0f, 0.0f, 0.1f, 0.07f);
  Object* pluto = new Object("Pluto.obj", 0, 0.0f, 0.0f, 0.1f, 0.07f);
  
  m_cubes.push_back(sun);
  m_cubes.push_back(mercury);
  m_cubes.push_back(venus);
  m_cubes.push_back(earth);
  m_cubes.push_back(earthMoon);
  m_cubes.push_back(mars);
  //m_cubes.push_back(marsMoonPhobos);
  //m_cubes.push_back(marsMoonDeimos);
  m_cubes.push_back(jupiter);
  //m_cubes.push_back(jupiterMoonGanymede);
  //m_cubes.push_back(jupiterCallisto);
  //m_cubes.push_back(jupiterMoonIo);
  //m_cubes.push_back(jupiterMoonEuropa);
  m_cubes.push_back(saturnRings);
  //m_cubes.push_back(saturnMoonTitan);
  //m_cubes.push_back(saturnMoonRhea);
  m_cubes.push_back(uranus);
  //m_cubes.push_back(uranusMoonOberon);
  //m_cubes.push_back(uranusMoonUmbriel);
  //m_cubes.push_back(uranusMoonAriel);
  m_cubes.push_back(neptune);
  //m_cubes.push_back(neptuneMoonTriton);
  //m_cubes.push_back(neptuneMoonProteus);
  m_cubes.push_back(pluto);

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  // Update the objects
  for(unsigned int i = 0; i < m_cubes.size(); i++)
  {
    m_cubes[i]->Update(dt);
  }
}

Camera* Graphics::getCamera()
{
   return m_camera;
}

Object* Graphics::GetObject(int index)
{
  return m_cubes[index];
}

int Graphics::numberOfCubes()
{
  return m_cubes.size();
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the objects
  for(unsigned int i = 0; i < m_cubes.size(); i++)
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cubes[i]->GetModel()));
    m_cubes[i]->Render();
  }

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}
