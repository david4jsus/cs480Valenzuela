#include "object.h"
#include <iostream>
#include <fstream>

using namespace std;

Object::Object()
{
	
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt)
{
  // Determine center
  glm::mat4 center;

  // Set position
  model = glm::translate(center, pos);
  
  // Pass this to any children objects
  modelForChild = model;
  
  // Physics
  btTransform trans;
  btScalar m[16];
  m_graphics->GetDynamicsWorld()->stepSimulation(dt, 10);
  rigidBody->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);
  
  btVector3 ballTransform = trans.getOrigin();
  
  pos = glm::vec3(ballTransform.getX(), ballTransform.getY(), ballTransform.getZ());
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetModelForChild()
{
  return modelForChild;
}

glm::vec3 Object::objectPosition()
{
   glm::vec3 objPos = pos;
   
   if (parent != 0)
   {
      objPos += parent->objectPosition();
   }
   
   return objPos;
}

void Object::setPosition(glm::vec3 newPos)
{
   pos = newPos;
}

std::string Object::GetObjectName()
{
   return objName;
}

btCollisionShape* Object::GetCollisionShape()
{
   return colliderShape;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,m_pos));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,m_tex));
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,m_normal));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  if (correctModelLoad)
  {
    // Bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    
    // Draw
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

btRigidBody* Object::GetRigidBody()
{
  return rigidBody;
}

bool Object::Init()
{
	m_pMesh = new Mesh();
	return m_pMesh->LoadMesh("../assets/models/awesomeball.obj");
}






