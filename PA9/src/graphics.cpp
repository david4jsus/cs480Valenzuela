#include "graphics.h"

Graphics::Graphics()
{
	broadphase = new btDbvtBroadphase();
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	
	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));
	
	shaderToggle = true;
	ambientLightingScale = 0.0;
}

Graphics::~Graphics()
{
    if( dynamicsWorld != NULL )
    {
        delete dynamicsWorld;

        dynamicsWorld = NULL;
    }

    if( solver != NULL )
    {
        delete solver;

        solver = NULL;
    }

    if( dispatcher != NULL )
    {
        delete dispatcher;

        dispatcher = NULL;
    }

    if( collisionConfig != NULL )
    {
        delete collisionConfig;

        collisionConfig = NULL;
    }

    if( broadphase != NULL )
    {
        delete broadphase;

        broadphase = NULL;
    }
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
  //Object* Skybox = new Object(this, "Skybox.obj",      0, 0.0f, 0.0f, 0.0f, 25.0f, 0, 0);
  Object* board  = new Object(this, "Disboard.obj",    0, 0.0f, 0.0f, 0.0f, 1.0f,  0, 0);
  Object* ball   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.01f, 1, 1);
  Object* cube = new Object(this, "cube.obj", 0, 0.0f, 0.0f, 0.0f, 0.5f, 1, 2);
  Object* cylinder = new Object(this, "cylinder.obj", 0, 0.0f, 0.0f, 0.0f, 0.5f, 0, 3);
  Object* backWall   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 4);
  Object* frontWall   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 5);
  Object* leftWall   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 6);
  Object* rightWall   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 7);

  // Waiting Song while the planets load
  gameSound.LoadSound("../assets/NGGUP.wav");
  gameSound.PlaySound();
  
  // Push objects onto list
  //m_cubes.push_back(Skybox);
  m_cubes.push_back(board);
  m_cubes.push_back(ball);
  m_cubes.push_back(cube);
  m_cubes.push_back(cylinder);
  m_cubes.push_back(backWall);
  m_cubes.push_back(frontWall);
  m_cubes.push_back(leftWall);
  m_cubes.push_back(rightWall);
  
  // get rigidbody for the cube
  rigidBody = m_cubes[2]->GetRigidBody();

  // PER VERTEX SHADER
  // Set up the shaders
  m_PerVertexShader = new Shader();
  if(!m_PerVertexShader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_PerVertexShader->AddShader(GL_VERTEX_SHADER, "../assets/shaders/vLightingVertex.shader"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_PerVertexShader->AddShader(GL_FRAGMENT_SHADER, "../assets/shaders/vLightingFragment.shader"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // PER VERTEX SHADER //
  
  // Connect the program
  if(!m_PerVertexShader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }
  
  // PER FRAGMENT SHADER
  // Setup the shaders
  m_PerFragmentShader = new Shader();
  if(!m_PerFragmentShader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_PerFragmentShader->AddShader(GL_VERTEX_SHADER, "../assets/shaders/fLightingVertex.shader"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_PerFragmentShader->AddShader(GL_FRAGMENT_SHADER, "../assets/shaders/fLightingFragment.shader"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_PerFragmentShader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the per vertex shader
  m_vprojectionMatrix = m_PerVertexShader->GetUniformLocation("projectionMatrix");
  if (m_vprojectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_vprojectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the per vertex shader
  m_vviewMatrix = m_PerVertexShader->GetUniformLocation("viewMatrix");
  if (m_vviewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_vviewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the per vertex shader
  m_vmodelMatrix = m_PerVertexShader->GetUniformLocation("modelMatrix");
  if (m_vmodelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_vmodelMatrix not found\n");
    return false;
  }
  
  // Locate the light position in the per vertex shader
  m_vlightPos = m_PerVertexShader->GetUniformLocation("lightPos");
  if (m_vlightPos == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_vlightPos not found\n");
    return false;
  }
  
  // Locate the ambient color in the per vertex shader
  m_vambientColor = m_PerVertexShader->GetUniformLocation("ambientColor");
  if (m_vlightPos == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_vambientColor not found\n");
    return false;
  }
  
  // Locate the diffuse color in the per fragment shader
  m_vdiffuseColor = m_PerFragmentShader->GetUniformLocation("diffuseColor");
  if (m_vdiffuseColor == INVALID_UNIFORM_LOCATION)
  {
    printf("m_vdiffuseColor not found\n");
    return false;
  }
  
  // Locate the specular color in the per fragment shader
  m_vspecularColor = m_PerFragmentShader->GetUniformLocation("specularColor");
  if (m_vspecularColor == INVALID_UNIFORM_LOCATION)
  {
    printf("m_vspecularColor not found\n");
    return false;
  }
  
  // Locate the shininess in the per fragment shader
  m_vshininess = m_PerFragmentShader->GetUniformLocation("shininess");
  if (m_vshininess == INVALID_UNIFORM_LOCATION)
  {
    printf("m_vshininess not found\n");
    return false;
  }
  
  // PER FRAGMENT SHADER //
  
  // Locate the projection matrix in the per fragment shader
  m_fprojectionMatrix = m_PerFragmentShader->GetUniformLocation("projectionMatrix");
  if (m_fprojectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_fprojectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the per fragment shader
  m_fviewMatrix = m_PerFragmentShader->GetUniformLocation("viewMatrix");
  if (m_fviewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_fviewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the per fragment shader
  m_fmodelMatrix = m_PerFragmentShader->GetUniformLocation("modelMatrix");
  if (m_fmodelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_fmodelMatrix not found\n");
    return false;
  }
  
  // Locate the light position in the per fragment shader
  m_flightPos = m_PerFragmentShader->GetUniformLocation("lightPos");
  if (m_flightPos == INVALID_UNIFORM_LOCATION)
  {
    printf("m_flightPos not found\n");
    return false;
  }
  
  // Locate the ambient color in the per fragment shader
  m_fambientColor = m_PerFragmentShader->GetUniformLocation("ambientColor");
  if (m_flightPos == INVALID_UNIFORM_LOCATION)
  {
    printf("m_fambientColor not found\n");
    return false;
  }
  
  // Locate the diffuse color in the per fragment shader
  m_fdiffuseColor = m_PerFragmentShader->GetUniformLocation("diffuseColor");
  if (m_fdiffuseColor == INVALID_UNIFORM_LOCATION)
  {
    printf("m_fdiffuseColor not found\n");
    return false;
  }
  
  // Locate the specular color in the per fragment shader
  m_fspecularColor = m_PerFragmentShader->GetUniformLocation("specularColor");
  if (m_fspecularColor == INVALID_UNIFORM_LOCATION)
  {
    printf("m_fspecularColor not found\n");
    return false;
  }
  
  // Locate the shininess in the per fragment shader
  m_fshininess = m_PerFragmentShader->GetUniformLocation("shininess");
  if (m_fshininess == INVALID_UNIFORM_LOCATION)
  {
    printf("m_fshininess not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::switchShaders()
{
	shaderToggle = !shaderToggle;
}

void Graphics::setVertexShader()
{
	shaderToggle = false;
}

void Graphics::setFragmentShader()
{
	shaderToggle = true;
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

btDiscreteDynamicsWorld* Graphics::GetDynamicsWorld()
{
  return dynamicsWorld;
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  if (shaderToggle) // PER FRAGMENT SHADERS
  {
	  m_PerFragmentShader->Enable();
	  
	  // Send in the projection and view to the shader
	  glUniformMatrix4fv(m_fprojectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	  glUniformMatrix4fv(m_fviewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	
	  // Render the objects
	  for(unsigned int i = 0; i < m_cubes.size(); i++)
	  {
		glUniformMatrix4fv(m_fmodelMatrix, 1, GL_FALSE, glm::value_ptr(m_cubes[i]->GetModel()));
		m_cubes[i]->Render();
	  }
	  
	  // Send light position
	  glUniform4f(m_flightPos, 25.0, 10.0, 0.0, 1.0);
	  
	  // Send ambient color
	  glUniform4f(m_fambientColor, ambientLightingScale, ambientLightingScale, ambientLightingScale, 1.0);
	  
	  // Send diffuse color
	  glUniform4f(m_fdiffuseColor, 1.0, 1.0, 1.0, 1.0);
	  
	  // Send specular color
	  glUniform4f(m_fspecularColor, 0.1, 0.1, 0.1, 1.0);
	  
	  // Send shininess
	  glUniform1f(m_fshininess, 0.5);
  }
  else // PER VERTEX SHADERS
  {
	  m_PerVertexShader->Enable();
	  
	  // Send in the projection and view to the shader
	  glUniformMatrix4fv(m_vprojectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
	  glUniformMatrix4fv(m_vviewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 
	
	  // Render the objects
	  for(unsigned int i = 0; i < m_cubes.size(); i++)
	  {
		glUniformMatrix4fv(m_vmodelMatrix, 1, GL_FALSE, glm::value_ptr(m_cubes[i]->GetModel()));
		m_cubes[i]->Render();
	  }
	  
	  // Send light position
	  glUniform4f(m_vlightPos, 25.0f, 5.0f, 0.0f, 1.0f);
	  
	  // Send ambient color
	  glUniform4f(m_vambientColor, ambientLightingScale, ambientLightingScale, ambientLightingScale, 1.0f);
	  
	  // Send diffuse color
	  glUniform4f(m_vdiffuseColor, 0.0f, 0.0f, 0.0f, 1.0f);
	  
	  // Send specular color
	  glUniform4f(m_vspecularColor, 0.0f, 0.0f, 0.0f, 1.0f);
	  
	  // Send shininess
	  glUniform1f(m_vshininess, 0.0f);
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

float Graphics::getAmbientLightingScale()
{
  return ambientLightingScale;
}

float Graphics::setAmbientLightingScale(float setAmbientLighting)
{
  ambientLightingScale = setAmbientLighting;
}
