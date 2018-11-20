#include "graphics.h"

Graphics::Graphics()
{
	// Initialize Bullet World
	broadphase = new btDbvtBroadphase();
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	
	dynamicsWorld->setGravity(btVector3(-0.001, -1.0, 0.0));
	
	shaderToggle = true;
	ambientLightingScale = 0.5;
	specularScale = 0.5;
}

Graphics::Graphics(string vLightingVertFilePath, string vLightingFragFilePath, string fLightingVertFilePath, string fLightingFragFilePath, glm::vec3 storedEngineStartingCameraPos, 
                   float storedEngineYaw, float storedEnginePitch, std::vector<ObjectInfo> allObjectsInfo) : Graphics()
{
  // per vertex lighting vertex and fragment shader filepaths
  vLightingVertexShaderFilePath = vLightingVertFilePath;
  vLightingFragmentShaderFilePath = vLightingFragFilePath;
  
  // per fragment lighting vertex and fragment shader filepaths
  fLightingVertexShaderFilePath = fLightingVertFilePath;
  fLightingFragmentShaderFilePath = fLightingFragFilePath;
  
  // camera starting pos
  storedGraphicsStartingCameraPos = storedEngineStartingCameraPos;
  
  // camera starting orientation
  storedGraphicsYaw = storedEngineYaw;
  storedGraphicsPitch = storedEnginePitch;
  
  // information required to create each object
  objectsInfo = allObjectsInfo;
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
    
    if (m_camera != NULL)
    {
    	delete m_camera;
    	m_camera = NULL;
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
  setCameraStartingPos();
  m_camera->updateCamRotYaw(storedGraphicsYaw);
  m_camera->updateCamRotPitch(storedGraphicsPitch);

  // Create objects
  for(int i = 0; i < objectsInfo.size(); i++)
  {
    m_objects.push_back(new Object(objectsInfo[i].objectName, glm::vec3(0, 0, 0), this, objectsInfo[i]));
  }


  meshLoaded = true;
  
  if(meshLoaded)
  {
	  // Waiting Song while the planets load
	  gameSound.LoadSound("../assets/NGGUP.wav");
	  gameSound.PlaySound();
	  
	  // get rigidbody for the cube
	  /*for(int i = 0; i < m_objects.size(); i++)
	  {
	    rigidBodies.push_back(m_objects[i]->GetRigidBody());
	  }*/

	  // PER VERTEX SHADER
	  // Set up the shaders
	  m_PerVertexShader = new Shader();
	  if(!m_PerVertexShader->Initialize())
	  {
	    printf("Shader Failed to Initialize\n");
	    return false;
	  }

	  // Add the vertex shader
	  if(!m_PerVertexShader->AddShader(GL_VERTEX_SHADER, vLightingVertexShaderFilePath))
	  {
	    printf("Vertex Shader failed to Initialize\n");
	    return false;
	  }

	  // Add the fragment shader
	  if(!m_PerVertexShader->AddShader(GL_FRAGMENT_SHADER, vLightingFragmentShaderFilePath))
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
	  if(!m_PerFragmentShader->AddShader(GL_VERTEX_SHADER, fLightingVertexShaderFilePath))
	  {
	    printf("Vertex Shader failed to Initialize\n");
	    return false;
	  }

	  // Add the fragment shader
	  if(!m_PerFragmentShader->AddShader(GL_FRAGMENT_SHADER, fLightingFragmentShaderFilePath))
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
	  if (m_vambientColor == INVALID_UNIFORM_LOCATION) 
	  {
	    printf("m_vambientColor not found\n");
	    return false;
	  }
	  
	  // Locate the diffuse color in the per fragment shader
	  m_vdiffuseColor = m_PerVertexShader->GetUniformLocation("diffuseColor");
	  if (m_vdiffuseColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vdiffuseColor not found\n");
	    return false;
	  }
	  
	  // Locate the specular color in the per fragment shader
	  m_vspecularColor = m_PerVertexShader->GetUniformLocation("specularColor");
	  if (m_vspecularColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vspecularColor not found\n");
	    return false;
	  }
	  
	  // Locate the shininess in the per fragment shader
	  m_vshininess = m_PerVertexShader->GetUniformLocation("shininess");
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
	  if (m_fambientColor == INVALID_UNIFORM_LOCATION)
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
  
  return false;
}

void Graphics::Update(unsigned int dt)
{
  // Update the objects
  	
  for(unsigned int i = 0; i < m_objects.size(); i++)
  {
    m_objects[i]->Update(dt);
  }
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
	  for(unsigned int i = 0; i < m_objects.size(); i++)
	  {
		  glUniformMatrix4fv(m_fmodelMatrix, 1, GL_FALSE, glm::value_ptr(m_objects[i]->GetModel()));
		  m_objects[i]->Render();
	  }
	  
	  // Send light position
	  glUniform4f(m_flightPos, 0, 5, 0, 1.0);
	  
	  // Send ambient color
	  glUniform4f(m_fambientColor, ambientLightingScale, ambientLightingScale, ambientLightingScale, 1.0);
	  
	  // Send diffuse color
	  glUniform4f(m_fdiffuseColor, 1.0, 1.0, 1.0, 1.0);
	  
	  // Send specular color
	  glUniform4f(m_fspecularColor, specularScale, specularScale, specularScale, 1.0);
	  
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
	  for(unsigned int i = 0; i < m_objects.size(); i++)
	  {
		  glUniformMatrix4fv(m_vmodelMatrix, 1, GL_FALSE, glm::value_ptr(m_objects[i]->GetModel()));
		  m_objects[i]->Render();
	  }
	  
	  // Send light position
	  glUniform4f(m_vlightPos, 0, 5, 0, 1.0);
	  
	  // Send ambient color
	  glUniform4f(m_vambientColor, ambientLightingScale, ambientLightingScale, ambientLightingScale, 1.0);
	  
	  // Send diffuse color
	  glUniform4f(m_vdiffuseColor, 1.0, 1.0, 1.0, 1.0);
	  
	  // Send specular color
	  glUniform4f(m_vspecularColor, specularScale, specularScale, specularScale, 1.0);
	  
	  // Send shininess
	  glUniform1f(m_vshininess, 0.5);
  }


  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

void Graphics::SwitchShaders()
{
	shaderToggle = !shaderToggle;
}

void Graphics::SetVertexShader()
{
	shaderToggle = false;
}

void Graphics::SetFragmentShader()
{
	shaderToggle = true;
}

Camera* Graphics::GetCamera()
{
   return m_camera;
}

Object* Graphics::GetObject(int index)
{
  return m_objects[index];
}

int Graphics::GetNumberOfObjects()
{
  return m_objects.size();
}

btDiscreteDynamicsWorld* Graphics::GetDynamicsWorld()
{
  return dynamicsWorld;
}

float Graphics::GetAmbientLightingScale()
{
	return ambientLightingScale;
}

float Graphics::GetSpecularScale()
{
	return specularScale;
}

float Graphics::SetAmbientLightingScale(float setAmbientLighting)
{
	ambientLightingScale = setAmbientLighting;
}

float Graphics::SetSpecularScale(float setSpecularScale)
{
	specularScale = setSpecularScale;
}

btRigidBody* Graphics::GetRigidBody(int objectIndex)
{
  return rigidBodies[objectIndex];
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

void Graphics::setCameraStartingPos()
{
  m_camera->setCamPos(storedGraphicsStartingCameraPos);
}
