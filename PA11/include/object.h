#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics.h"
#include "mesh.h"

#include <btBulletDynamicsCommon.h>

class Graphics;

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();
    
    // Test
    bool Init();

    glm::mat4 GetModel();
    glm::mat4 GetModelForChild();
    
    // Position of an object in space
    glm::vec3 objectPosition();
    
    void setPosition(glm::vec3 newPos);
    
    // Name of object
    std::string GetObjectName();
    
    // Collider getter
    btCollisionShape* GetCollisionShape();
    
    // rigidbody getter
    btRigidBody* GetRigidBody();

  private:
    Graphics* m_graphics;
    std::string objName;
    glm::mat4 model;
    glm::vec3 pos;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint Texture;
    
    // Collider
    btCollisionShape *colliderShape;
    btTriangleMesh *objTriMesh;
    btRigidBody* rigidBody;
    int m_mass;
    
    // If the object has a child, this is the matrix it should read as its center
    glm::mat4 modelForChild;

    Object* parent;
    
    // Object loading
    Mesh* m_pMesh;
    std::string objFilePath;
    bool correctModelLoad;

	
	  int modelNum;
	
	  glm::vec3 objectPos;
};

#endif /* OBJECT_H */
