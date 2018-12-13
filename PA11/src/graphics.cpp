#include "graphics.h"
#include <iostream>

using namespace std;

Graphics::Graphics()
{
	// Initialize Bullet World
	/*broadphase = new btDbvtBroadphase();
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	
	dynamicsWorld->setGravity(btVector3(-0.001, -1.0, 0.0));*/
	
	shaderToggle = true;
	ambientLightingScale = 0.5;
	specularScale = 0.5;
	
	m_physics = new Physics(this);
}

Graphics::Graphics(string vLightingVertFilePath, string vLightingFragFilePath, string fLightingVertFilePath, string fLightingFragFilePath, glm::vec3 storedEngineStartingCameraPos, 
                   float storedEngineYaw, float storedEnginePitch, btVector3 gravityDirection, float jumpHeight, std::vector<ObjectInfo> allObjectsInfo) : Graphics()
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
  
  // Initial light positions
  light1Pos = glm::vec3( 25, 5, 0);
  light2Pos = glm::vec3(-25, 5, 0);

	// get gravity direction
	this->gravityDirection = gravityDirection;

	// get jump height
	this->jumpHeight = jumpHeight;
}

Graphics::~Graphics()
{
    if (m_camera != NULL)
    {
    	delete m_camera;
    	m_camera = NULL;
    }
	
	if (m_physics != NULL)
	{
		delete m_physics;
		m_physics = NULL;
	}
}

bool Graphics::Initialize(int width, int height, std::string file)
{
  // Initialize physics
  m_physics->Initialize();
  
  // local variables
  int objectsLooper;

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
  for(objectsLooper = 0; objectsLooper < objectsInfo.size(); objectsLooper++)
  {
    m_objects.push_back(new Object(objectsInfo[objectsLooper].objectName, this, objectsInfo[objectsLooper]));
  }

  meshLoaded = true;
  
  if(meshLoaded)
  {
	  // Waiting Song while the planets load
	  gameSound.LoadSound("../assets/NGGUP.wav");
	  gameSound.PlaySound();
	  
	  // get rigidbodies from objects
	  for(objectsLooper = 0; objectsLooper < objectsInfo.size(); objectsLooper++)
	  {
	    m_physics->AddRigidBody(m_objects[objectsLooper]->GetRigidBody());
	  }

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

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/* VERTEX SHADER */
		/////////////////////////////////////////////////////////////////////////////////////////////////////

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
	  
		// Locate the light position in the per fragment shader
	  m_vFirstLightPos = m_PerVertexShader->GetUniformLocation("firstLightPos");
	  if (m_vFirstLightPos == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vlightPos not found\n");
	    return false;
	  }
	  
	  // Locate the ambient color in the per fragment shader
	  m_vambientColor = m_PerVertexShader->GetUniformLocation("ambientColor");
	  if (m_fambientColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vambientColor not found\n");
	    return false;
	  }
	  
	  // Locate the diffuse color in the per fragment shader
	  m_vFirstLightDiffuseColor = m_PerVertexShader->GetUniformLocation("diffuseColorFirstLight");
	  if (m_vFirstLightDiffuseColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vdiffuseColor not found\n");
	    return false;
	  }
	  
	  // Locate the specular color in the per fragment shader
	  m_vFirstLightSpecularColor = m_PerVertexShader->GetUniformLocation("specularColorFirstLight");
	  if (m_vFirstLightSpecularColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vspecularColor not found\n");
	    return false;
	  }
	  
	  // Locate the shininess in the per fragment shader
	  m_vFirstLightShininess = m_PerVertexShader->GetUniformLocation("shininessFirstLight");
	  if (m_vFirstLightShininess == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vshininess not found\n");
	    return false;
	  }  

		// Locate the light position in the per fragment shader
	  m_vSecondLightPos = m_PerVertexShader->GetUniformLocation("SecondLightPos");
	  if (m_vSecondLightPos == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vlightPos not found\n");
	    return false;
	  }
	  
	  // Locate the diffuse color in the per fragment shader
	  m_vSecondLightDiffuseColor = m_PerVertexShader->GetUniformLocation("diffuseColorSecondLight");
	  if (m_vSecondLightDiffuseColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vdiffuseColor not found\n");
	    return false;
	  }
	  
	  // Locate the specular color in the per fragment shader
	  m_vSecondLightSpecularColor = m_PerVertexShader->GetUniformLocation("specularColorSecondLight");
	  if (m_vSecondLightSpecularColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vspecularColor not found\n");
	    return false;
	  }
	  
	  // Locate the shininess in the per fragment shader
	  m_vSecondLightShininess = m_PerVertexShader->GetUniformLocation("shininessSecondLight");
	  if (m_vSecondLightShininess == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_vshininess not found\n");
	    return false;
	  }  

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/* FRAGMENT SHADER */
		/////////////////////////////////////////////////////////////////////////////////////////////////////

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
	  m_fFirstLightPos = m_PerFragmentShader->GetUniformLocation("firstLightPos");
	  if (m_fFirstLightPos == INVALID_UNIFORM_LOCATION)
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
	  m_fFirstLightDiffuseColor = m_PerFragmentShader->GetUniformLocation("diffuseColorFirstLight");
	  if (m_fFirstLightDiffuseColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_fdiffuseColor not found\n");
	    return false;
	  }
	  
	  // Locate the specular color in the per fragment shader
	  m_fFirstLightSpecularColor = m_PerFragmentShader->GetUniformLocation("specularColorFirstLight");
	  if (m_fFirstLightSpecularColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_fspecularColor not found\n");
	    return false;
	  }
	  
	  // Locate the shininess in the per fragment shader
	  m_fFirstLightShininess = m_PerFragmentShader->GetUniformLocation("shininessFirstLight");
	  if (m_fFirstLightShininess == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_fshininess not found\n");
	    return false;
	  }  

	  // Locate the light position in the per fragment shader
	  m_fSecondLightPos = m_PerFragmentShader->GetUniformLocation("secondLightPos");
	  if (m_fSecondLightPos == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_flightPos not found\n");
	    return false;
	  }
	  
		// Locate the diffuse color in the per fragment shader
	  m_fSecondLightDiffuseColor = m_PerFragmentShader->GetUniformLocation("diffuseColorSecondLight");
	  if (m_fSecondLightDiffuseColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_fdiffuseColor not found\n");
	    return false;
	  }
	  
	  // Locate the specular color in the per fragment shader
	  m_fSecondLightSpecularColor = m_PerFragmentShader->GetUniformLocation("specularColorSecondLight");
	  if (m_fSecondLightSpecularColor == INVALID_UNIFORM_LOCATION)
	  {
	    printf("m_fspecularColor not found\n");
	    return false;
	  }
	  
	  // Locate the shininess in the per fragment shader
	  m_fSecondLightShininess = m_PerFragmentShader->GetUniformLocation("shininessSecondLight");
	  if (m_fSecondLightShininess == INVALID_UNIFORM_LOCATION)
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
  
  m_physics->Update();

	// reposition player 1's weapon 
	//GetRigidBody("Player1")->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
	//btMotionState* playerOneBody = GetRigidBody("Player1")->getMotionState();
	//btTransform playerOneOrientationPosiiton;
	//playerOneBody->getWorldTransform(playerOneOrientationPosiiton);
	//GetRigidBody("JousterPlayer1")->setCenterOfMassTransform(playerOneOrientationPosiiton);
	//GetRigidBody("JousterPlayer1")->setWorldTransform(playerOneOrientationPosiiton);
	//GetRigidBody("JousterPlayer1")->getMotionState()->setWorldTransform(playerOneOrientationPosiiton);
	//GetRigidBody("JousterPlayer1")->setMotionState(playerOneBody);

	//btMotionState* playerTwoBody = GetRigidBody("Player2")->getMotionState();
	//btTransform playerTwoOrientationPosiiton;
	//playerTwoBody->getWorldTransform(playerTwoOrientationPosiiton);
	//GetRigidBody("JousterPlayer2")->setCenterOfMassTransform(playerTwoOrientationPosiiton);
	
	// Update light positions per player
	light1Pos = GetObjectByName("Player1")->GetObjectPosition();
	light2Pos = GetObjectByName("Player2")->GetObjectPosition();
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
	  glUniform4f(m_fFirstLightPos, light1Pos.x, light1Pos.y, light1Pos.z, 1.0);
	  
	  // Send ambient color
	  glUniform4f(m_fambientColor, ambientLightingScale, ambientLightingScale, ambientLightingScale, 1.0);
	  
	  // Send diffuse color
	  glUniform4f(m_fFirstLightDiffuseColor, 1.0, 1.0, 1.0, 1.0);
	  
	  // Send specular color
	  glUniform4f(m_fFirstLightSpecularColor, specularScale, specularScale, specularScale, 1.0);
	  
	  // Send shininess
	  glUniform1f(m_fFirstLightShininess, 0.5);

		// Send light position
	  glUniform4f(m_fSecondLightPos, light2Pos.x, light2Pos.y, light2Pos.z, 1.0);

	  // Send ambient color
	  glUniform4f(m_fambientColor, ambientLightingScale, ambientLightingScale, ambientLightingScale, 1.0);
	  
	  // Send diffuse color
	  glUniform4f(m_fSecondLightDiffuseColor, 1.0, 1.0, 1.0, 1.0);
	  
	  // Send specular color
	  glUniform4f(m_fSecondLightSpecularColor, specularScale, specularScale, specularScale, 1.0);
	  
	  // Send shininess
	  glUniform1f(m_fSecondLightShininess, 0.5);
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
	  glUniform4f(m_vFirstLightPos, 0, 5, 0, 1.0);
	  
	  // Send ambient color
	  glUniform4f(m_vambientColor, ambientLightingScale, ambientLightingScale, ambientLightingScale, 1.0);
	  
	  // Send diffuse color
	  glUniform4f(m_vFirstLightDiffuseColor, 1.0, 1.0, 1.0, 1.0);
	  
	  // Send specular color
	  glUniform4f(m_vFirstLightSpecularColor, specularScale, specularScale, specularScale, 1.0);
	  
	  // Send shininess
	  glUniform1f(m_vFirstLightShininess, 0.5);

	  // Send light position
	  glUniform4f(m_vSecondLightPos, 0, 5, 0, 1.0);
	  
	  // Send ambient color
	  glUniform4f(m_vambientColor, ambientLightingScale, ambientLightingScale, ambientLightingScale, 1.0);
	  
	  // Send diffuse color
	  glUniform4f(m_vSecondLightDiffuseColor, 1.0, 1.0, 1.0, 1.0);
	  
	  // Send specular color
	  glUniform4f(m_vSecondLightSpecularColor, specularScale, specularScale, specularScale, 1.0);
	  
	  // Send shininess
	  glUniform1f(m_vSecondLightShininess, 0.5);
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

Object* Graphics::GetObjectByName(string name)
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		Object* obj = GetObject(i);
		if (obj->GetObjectName() == name)
		{
			return obj;
		}
	}
	
	return NULL;
}

int Graphics::GetNumberOfObjects()
{
  return m_objects.size();
}

btDiscreteDynamicsWorld* Graphics::GetDynamicsWorld()
{
  return m_physics->GetDynamicsWorld();
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

btRigidBody* Graphics::GetRigidBody(string objectName)
{
  // local variables
  int objectsLooper;

  // loop through all objects
  for(objectsLooper = 0; objectsLooper < m_objects.size(); objectsLooper++)
  {
    // check if we found correct object
    if(m_objects[objectsLooper]->GetObjectName() == objectName)
    {
      // retrun orbject's rigid body
      return m_physics->GetObjectRigidBody(objectsLooper);
    }
  }

  return NULL;
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

void Graphics::setPlayerSettings(PlayerSettings* players)
{
	m_physics->setPlayerSettings(players);
}

btVector3 Graphics::getGravity()
{
	return gravityDirection;
}

float Graphics::getJumpHeight()
{
	return jumpHeight;
}

void Graphics::checkPlayerTwoJump()
{
	m_physics->getPlayerTwoCanJump();
}