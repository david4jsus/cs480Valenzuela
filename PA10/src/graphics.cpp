#include "graphics.h"

Graphics::Graphics()
{
	broadphase = new btDbvtBroadphase();
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	collisionWorld = new btCollisionWorld(dispatcher, broadphase, collisionConfig);
	
	dynamicsWorld->setGravity(btVector3(-0.001, -1.0, 0.0));
	
	shaderToggle = true;
	ambientLightingScale = 1.0;
	specularScale = 0.5f;
	pinballPos = glm::vec3(0.0, 0.0, 0.0);
	score = 0;
	lives = 3;
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
  // bottom of the pinball machine
  Object* base  = new Object(this, "Base.obj",    0, 0.0f, 0.0f, 0.0f, 1.0f,  0, 10);
  
  // pinball
  Object* ball   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.1f, 1, 1);
  
  // blockers
  Object* blockers = new Object(this, "Blockers.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // backboard
  Object* backBoard = new Object(this, "Backboard.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  Object* innerBackBoard = new Object(this, "InnerBackboard.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // barrier
  Object* barrier = new Object(this, "Barrier.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // bumpers
  Object* bumper2 = new Object(this, "Bumper_2.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // paddles
  Object* leftPaddle = new Object(this, "LeftPaddle.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 12);
  Object* rightPaddle = new Object(this, "RightPaddle.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 12);
  
  leftPaddle->GetRigidBody()->setGravity(btVector3(0.0f, 0.0, 0.0f));
  rightPaddle->GetRigidBody()->setGravity(btVector3(0.0f, 0.0, 0.0f));
  
  // pinball machine chasis
  Object* pinballBody = new Object(this, "PinballBody.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // plunger
  Object* plungerBase = new Object(this, "PlungerBase.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  Object* plungerHead = new Object(this, "PlungerHead.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // side triggers
  Object* sideTriggers = new Object(this, "SideTriggers.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // speakers
  Object* speakers = new Object(this, "Speakers.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // titles
  Object* title = new Object(this, "Title.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  Object* titleBackboard = new Object(this, "TitleBackboard.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // pinball obstacle
  Object* pinballHole = new Object(this, "PinballHole.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // front walls
  Object* wallsFront = new Object(this, "WallsFront.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  // upper level objects
  Object* bumper1 = new Object(this, "Bumper_1.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  Object* bumper3 = new Object(this, "Bumper_3.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  Object* bumper4 = new Object(this, "Bumper_4.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  Object* ballRamp = new Object(this, "BallRamp.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  Object* ramp = new Object(this, "Ramp.obj", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 10);
  
  
  Object* baseCollisionPosition   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0);
  //Object* plungerCollisionPosition   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 2);
  Object* bumperCollisionPosition   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 3);
  Object* backWall   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 4);
  Object* frontWall   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 5);
  Object* leftWall   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 6);
  Object* rightWall   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 7);
  Object* testBall   = new Object(this, "awesomeball.obj", 0, 0.0f, 0.0f, 0.0f, 0.1f, 1, 9);
  testBall->GetRigidBody()->setGravity(btVector3(0.0f, 0.0f, 0.0f));
  
  // Push objects onto list
  m_cubes.push_back(base);  // 0
  m_cubes.push_back(ball);  // 1
  m_cubes.push_back(blockers);  // 2 
  m_cubes.push_back(backBoard); // 3
  m_cubes.push_back(innerBackBoard);  // 4
  m_cubes.push_back(barrier); // 5
  m_cubes.push_back(bumper2); // 6
  m_cubes.push_back(leftPaddle);  // 7
  m_cubes.push_back(rightPaddle); // 8
  m_cubes.push_back(pinballBody); // 9
  m_cubes.push_back(plungerBase); // 10
  m_cubes.push_back(plungerHead); // 11
  m_cubes.push_back(sideTriggers);  // 12
  m_cubes.push_back(speakers);  // 13
  m_cubes.push_back(title); // 14
  m_cubes.push_back(titleBackboard);  // 15
  m_cubes.push_back(pinballHole);  // 16
  m_cubes.push_back(wallsFront);  // 17
  //m_cubes.push_back(testBall);
  m_cubes.push_back(bumperCollisionPosition); // 18
  m_cubes.push_back(bumper1); 
  m_cubes.push_back(bumper3); 
  m_cubes.push_back(bumper4); 
  m_cubes.push_back(ballRamp); 
  m_cubes.push_back(ramp);  
      
  gameSound.LoadSound("../assets/Ryuusei.wav");
  gameSound.PlaySound();
  
  // get rigidbody for the cube
  for(int i = 0; i < m_cubes.size(); i++)
  {
    rigidBodies.push_back(m_cubes[i]->GetRigidBody());
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
  
  // Get the pinball position every frame
  pinballPos = m_cubes[1]->objectPosition();
  
  // Set the spot light position to the pinball position
  //glUniform4f(m_flightPos, pinballPos.x, pinballPos.y, pinballPos.z, 1.0);
  
	
	
	
	/////////////////////////////////////////////////////////////////////////////
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	/////////////////////////////////////////////////////////////////////////////
	
	
	
	
  // Collision detection
  collisionWorld->performDiscreteCollisionDetection();
  
  int numManifolds = collisionWorld->getDispatcher()->getNumManifolds();
  
  //cout << numManifolds << endl;
  //For each contact manifold
  /*for (int i = 0; i < numManifolds; i++)
  {
    btPersistentManifold* contactManifold = collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
    const btCollisionObject* obA = contactManifold->getBody0();
    const btCollisionObject* obB = contactManifold->getBody1();
    contactManifold->refreshContactPoints(obA->getWorldTransform(), obB->getWorldTransform());
    int numContacts = contactManifold->getNumContacts();
    //For each contact point in that manifold
    for (int j = 0; j < numContacts; j++)
    {
      const btCollisionShape* bumper = m_cubes[17]->GetCollisionShape();
      const btCollisionShape* ball = m_cubes[1]->GetCollisionShape();
      if (obA->getCollisionShape() == ball && obB->getCollisionShape() == bumper)
      {
         cout << "||" << "Ball collision with front wall" << endl;
      }
    }
  }*/
  if(numManifolds == 16)
  {
    score = score + 100; 
    hitSound.LoadSound("../assets/grenade.wav");
    hitSound.PlayBumperHit();
  }
	
	/////////////////////////////////////////////////////////////////////////////
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	// HERE // HERE // HERE // HERE //HERE //HERE //HERE //HERE //HERE // HERE //
	/////////////////////////////////////////////////////////////////////////////
	
	
	
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
	  glUniform4f(m_flightPos, pinballPos.x, pinballPos.y + 1, pinballPos.z, 1.0);
	  //glUniform4f(m_flightPos, 0, 0, 0, 1.0);
	  
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
	  for(unsigned int i = 0; i < m_cubes.size(); i++)
	  {
		  glUniformMatrix4fv(m_vmodelMatrix, 1, GL_FALSE, glm::value_ptr(m_cubes[i]->GetModel()));
		  m_cubes[i]->Render();
	  }
	  
	  // Send light position
	  glUniform4f(m_vlightPos, pinballPos.x, pinballPos.y + 1, pinballPos.z, 1.0);
	  //glUniform4f(m_vlightPos, 0, 0, 0, 1.0);
	  
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

float Graphics::GetSpecularScale()
{
	return specularScale;
}

float Graphics::setAmbientLightingScale(float setAmbientLighting)
{
	ambientLightingScale = setAmbientLighting;
}

float Graphics::SetSpecularScale(float setSpecularScale)
{
	specularScale = setSpecularScale;
}

btRigidBody* Graphics::getRigidBody(int objectIndex)
{
  return rigidBodies[objectIndex];
}

int Graphics::getScore()
{
  return score;
}

int Graphics::GetLives()
{
  return lives;
}
