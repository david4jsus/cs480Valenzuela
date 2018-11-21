#include "physics.h"

//== Parameterzide constructor ==//
Physics::Physics(Graphics* graphics)
{
	m_graphics = graphics;
	
	broadphase = 0;
	collisionConfig = 0;
	dispatcher = 0;
	solver = 0;
	dynamicsWorld = 0;
}

//== Destructor ==//
Physics::~Physics()
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

//== Initializer ==//
bool Physics::Initialize()
{
	broadphase = new btDbvtBroadphase();
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	
	dynamicsWorld->setGravity(btVector3(-0.001, -1.0, 0.0));
}

//== Update function ==//
void Physics::Update(/*unsigned int dt*/)
{
	CheckCollisions();
}

//== Discrete dynamics world pointer getters ==//
btDiscreteDynamicsWorld* Physics::GetDynamicsWorld()
{
	return dynamicsWorld;
}

//== Rigid bodies getter ==//
btRigidBody* Physics::GetObjectRigidBody(unsigned int index)
{
	// Return the corresponding rigid body only if it exists
	if (index >= 0 && index < rigidBodies.size())
	{
	  return rigidBodies[index];
	}
	
	return NULL;
}

//== Add rigid bodies ==//
void Physics::AddRigidBody(btRigidBody* rigidBody)
{
	rigidBodies.push_back(rigidBody);
}

//== Check for collisions ==//
void Physics::CheckCollisions()
{
	// Stuff
}