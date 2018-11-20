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
    Object(std::string filename, glm::vec3 position, float size, Graphics* graphics);
    ~Object();
    void Update(unsigned int dt);
    void Render();
    
    // Initialize object
    bool Init();

    // Object model matrices getters
    glm::mat4 GetModel();
    glm::mat4 GetModelForChild();
    
    // Object position getter & setter
    glm::vec3 GetObjectPosition();
    void setPosition(glm::vec3 newPos);
    
    // Object name getter
    std::string GetObjectName();
    
    // Object collider getter
    btCollisionShape* GetCollisionShape();
    
    // Object Rigidbody getter
    btRigidBody* GetRigidBody();

  private:
    // Pointer to graphics class
    Graphics* m_graphics;

    // Object info
    std::string objectName;
	glm::vec3 objectPosition;
	float objectSize;
    int modelNum;
    
    // If the object has a child, this is the matrix it should read as its center
    glm::mat4 modelForChild;
    Object* parent;
    
    // Object loading
    Mesh* m_pMesh;
    std::string objFilePath;
    
    // Model info
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint Texture;
    glm::mat4 model;
    
    // Collider
    btCollisionShape *colliderShape;
    btTriangleMesh *objTriMesh;
    btRigidBody* rigidBody;
    int m_mass;
};

#endif /* OBJECT_H */
