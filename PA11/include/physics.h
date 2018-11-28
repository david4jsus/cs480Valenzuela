#ifndef PHYSICS_H
#define PHYSICS_H

#include "graphics.h"
#include "playerSettings.h"

#include <btBulletDynamicsCommon.h>

#include <chrono>

using namespace std::chrono;

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

		// sets player conditions such as lives remaining
		void setPlayerSettings(PlayerSettings* players);
  
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

		// player settings
    PlayerSettings* players;

		// invincibility time
    high_resolution_clock::time_point startTime;
    high_resolution_clock::time_point endTime;
		duration<double, std::milli> time_span;
};

#endif // PHYSICS_H
