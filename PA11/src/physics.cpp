#include "physics.h"
#include <iostream>

using namespace std;

//== Parameterzide constructor ==//
Physics::Physics(Graphics* graphics)
{
	m_graphics = graphics;
	
	broadphase = 0;
	collisionConfig = 0;
	dispatcher = 0;
	solver = 0;
	dynamicsWorld = 0;
	collisionWorld = 0;

	startTimePlayerOneInvincibility = high_resolution_clock::now();
	endTimePlayerOneInvincibility = high_resolution_clock::now();
	startTimePlayerTwoInvincibility = high_resolution_clock::now();
	endTimePlayerTwoInvincibility = high_resolution_clock::now();
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
	collisionWorld = new btCollisionWorld(dispatcher, broadphase, collisionConfig);
	
	dynamicsWorld->setGravity(m_graphics->getGravity());
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
	// local variables
	int playerOneRemainingLives, playerTwoRemainingLives;	

	collisionWorld->performDiscreteCollisionDetection();
	
	int numManifolds = collisionWorld->getDispatcher()->getNumManifolds();
	
   // Testing Constraints
	//btTypedConstraint* p2p = new btPoint2PointConstraint(*m_graphics->GetRigidBody("JoustTip"), btVector3(0,-1.0,0));
	//dynamicsWorld->addConstraint(p2p);
	
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();
		contactManifold->refreshContactPoints(obA->getWorldTransform(), obB->getWorldTransform());
		int numContacts = contactManifold->getNumContacts();
		
		if (numContacts > 0)
		{
			string obAName, obBName;
			const btCollisionShape* obAShape = obA->getCollisionShape();
			const btCollisionShape* obBShape = obB->getCollisionShape();
			
			for (int j = 0; j < rigidBodies.size(); j++)
			{
				if (rigidBodies[j]->getCollisionShape() == obAShape)
				{
					obAName = m_graphics->GetObject(j)->GetObjectName();
				}
				else if (rigidBodies[j]->getCollisionShape() == obBShape)
				{
					obBName = m_graphics->GetObject(j)->GetObjectName();
				}
			}
			
			// get current amount of remaining player lives
			players->getPlayersLives(playerOneRemainingLives, playerTwoRemainingLives);

			// calcualte remaining lives if neither player is dead
			if(playerOneRemainingLives != 0 && playerTwoRemainingLives != 0)
			{
				// calculate player 1 invincibility period
				endTimePlayerOneInvincibility = high_resolution_clock::now();
				time_span = endTimePlayerOneInvincibility - startTimePlayerOneInvincibility;

				// decrement each players lives
			   // Player 1
				/*if(players->Stabbed())
				{
				   playerTwoRemainingLives--;
				}
				
				else
				{
				   playerOneRemainingLives--;
				}*/
				
				// update remaining amount of lives
				players->setPlayersLives(playerOneRemainingLives, playerTwoRemainingLives);
				
				// check if player 1's weapons hit player 2's body
				if ((obAName == "DeathZone" || obAName == "Player1") && (obBName == "Player1" || obBName == "DeathZone"))
				{
					if(time_span.count() > players->getInvincibilityTime())
					{
						cout << "|| Collision!" << endl;

						// decrement each players lives
						playerOneRemainingLives--;

						// update remaining amount of lives
						players->setPlayersLives(playerOneRemainingLives, playerTwoRemainingLives);

						// restart invincibility period
						startTimePlayerOneInvincibility = high_resolution_clock::now();

						// reset player 2's position
						m_graphics->GetRigidBody("Player1")->setCenterOfMassTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(10, 10, 0)));
					}

					else if(time_span.count() <= players->getInvincibilityTime())
					{
						cout << "|| Collision!" << endl;

						// reset player 2's position
						m_graphics->GetRigidBody("Player1")->setCenterOfMassTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(10, 10, 0)));
					}
				}

			// calculate player 2 invincibility period
			endTimePlayerTwoInvincibility = high_resolution_clock::now();
			time_span = endTimePlayerTwoInvincibility - startTimePlayerTwoInvincibility;

			// check if player 2's weapons hit player 1's body
			if ((obAName == "DeathZone" || obAName == "Player2") && (obBName == "Player2" || obBName == "DeathZone"))
				{
					if(time_span.count() > players->getInvincibilityTime())
					{
						cout << "|| Collision!" << endl;

						// decrement each players lives
							// will be changed in the future
						playerTwoRemainingLives--;

						// update remaining amount of lives
						players->setPlayersLives(playerOneRemainingLives, playerTwoRemainingLives);

						// restart invincibility period
						startTimePlayerTwoInvincibility = high_resolution_clock::now();

						// reset player 1's position
						m_graphics->GetRigidBody("Player2")->setCenterOfMassTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-10, 10, 0)));
					}

					else if(time_span.count() <= players->getInvincibilityTime())
					{
						cout << "|| Collision!" << endl;

						// reset player 2's position
						m_graphics->GetRigidBody("Player2")->setCenterOfMassTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-10, 10, 0)));
					}
				}
			}
		}
	}
}

void Physics::setPlayerSettings(PlayerSettings* players)
{
  this->players = players;
}
