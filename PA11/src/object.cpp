#include "object.h"
#include <iostream>
#include <fstream>

using namespace std;

Object::Object()
{
    // Pointer to graphics class
    m_graphics = 0;

    // Object info
    objectName = "";
    objectScale = 1;
    modelNum = -1;
    
    // If the object has a child, this is the matrix it should read as its center
    parent = 0;
    
    // Object loading
    m_pMesh = 0;
    objFilePath = "";
    
    // Collider
    colliderShape = 0;
    objTriMesh = 0;
    rigidBody = 0;
    m_mass = 0;
}

Object::Object(std::string filename, Graphics* graphics, ObjectInfo anObject) : Object()
{
	// Set variables
	float deafaultInertia = 0.0;
	float defaultBounciness = 1.0;
	m_graphics = graphics;
	objFilePath = "../assets/models/" + filename;
	objectPosition = glm::vec3(anObject.objectPos.getX(), anObject.objectPos.getY(), anObject.objectPos.getZ());
	objectScale = anObject.scale;
	
	objectName = filename;
	objectName.erase(objectName.end() - 4, objectName.end());
	
	// motion state 
    btDefaultMotionState *shapeMotionState;
	
	// check if collision shape type is a box
	if(anObject.collisionShapeType == "box")
	{
	  // create a box collider
	  colliderShape = new btBoxShape(anObject.boxSize);
	  
	  // set orientation and position of object
	  shapeMotionState = new btDefaultMotionState(btTransform(anObject.objectOrientation, anObject.objectPos));
	}
	
	// check if collision shape type is a sphere
	else if(anObject.collisionShapeType == "sphere")
	{
	  // create a sphere collider
	  colliderShape = new btSphereShape(anObject.sphereRadius);
	  
	  // set orientation and position of object
	  shapeMotionState = new btDefaultMotionState(btTransform(anObject.objectOrientation, anObject.objectPos));
	}
	
	// check if collision shape type is a plane
    else if(anObject.collisionShapeType == "plane")
	{
	  // create a plane collider
      colliderShape = new btStaticPlaneShape(anObject.planeDirection, anObject.planeConstant);
    
    // set orientation and position of object
    shapeMotionState = new btDefaultMotionState(btTransform(anObject.objectOrientation, anObject.objectPos));
	}
	
	// set mass and inertia
  btScalar mass(anObject.mass);
  btVector3 inertia(deafaultInertia, deafaultInertia, deafaultInertia);
  
  // calculate inertia
  colliderShape->calculateLocalInertia(mass, inertia);
  
  // brind together all data needed to create a rigidbody
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, colliderShape, inertia);
  
  // create rigidbody
  rigidBody = new btRigidBody(shapeRigidBodyCI);
  
  // set bounciness of rigidbody
  rigidBody->setRestitution(defaultBounciness);
  
  // add rigidbody to world
  m_graphics->GetDynamicsWorld()->addRigidBody(rigidBody);
  
  // disable the deactivation of the rigidbody
  rigidBody->setActivationState(DISABLE_DEACTIVATION);
  
	
	// Initialize object
	if (!Init())
	{
		cerr << "ERROR: Mesh for " << objectName << " could not be loaded!" << endl;
	}
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt)
{  
  // Physics
  btTransform trans;
  btScalar m[16];
  m_graphics->GetDynamicsWorld()->stepSimulation(dt, 10);
  rigidBody->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);
  model = glm::scale(model, glm::vec3(objectScale, objectScale, objectScale));
  
  btVector3 transform = trans.getOrigin();
  
  objectPosition = glm::vec3(transform.getX(), transform.getY(), transform.getZ());
}

void Object::Render()
{  
  m_pMesh->Render();
}

bool Object::Init()
{
	m_pMesh = new Mesh();
	return m_pMesh->LoadMesh(objFilePath);
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetModelForChild()
{
  return modelForChild;
}

glm::vec3 Object::GetObjectPosition()
{
   glm::vec3 objPos = objectPosition;
   
   if (parent != 0)
   {
      objPos += parent->GetObjectPosition();
   }
   
   return objPos;
}

void Object::setPosition(glm::vec3 newPos)
{
   objectPosition = newPos;
   
   btTransform transform;
   rigidBody->getMotionState()->getWorldTransform(transform);
   transform.setOrigin(btVector3(objectPosition.x, objectPosition.y, objectPosition.z));
   rigidBody->getMotionState()->setWorldTransform(transform);
}

std::string Object::GetObjectName()
{
   return objectName;
}

btCollisionShape* Object::GetCollisionShape()
{
   return colliderShape;
}

btRigidBody* Object::GetRigidBody()
{
  return rigidBody;
}
