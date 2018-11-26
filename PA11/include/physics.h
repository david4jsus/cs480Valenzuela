#ifndef PHYSICS_H
#define PHYSICS_H

#include "graphics.h"

#include <btBulletDynamicsCommon.h>

class Graphics;

class Physics
{
  public:
	//Physics();
    Physics(Graphics* graphics);
    ~Physics();
    bool Initialize();
    void Update(/*unsigned int deltaTime*/);
    
    // Getters
    btDiscreteDynamicsWorld* GetDynamicsWorld();
    btRigidBody* GetObjectRigidBody(unsigned int objectName);
    
    // Add rigid bodies
    void AddRigidBody(btRigidBody* rigidBody);
  
  private:
    // Admin variables
    Graphics *m_graphics;
    
    // Bullet variables
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    btCollisionWorld* collisionWorld;
    
    // List of rigid bodies
    std::vector<btRigidBody*> rigidBodies;
    
    // Other functions
    void CheckCollisions();
};

#endif // PHYSICS_H
