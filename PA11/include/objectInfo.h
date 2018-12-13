#ifndef OBJECT_INFO_CPP
#define OBJECT_INFO_CPP

#include <btBulletDynamicsCommon.h>

struct ObjectInfo
{
    // object name
    string objectName;
    
    // object position
    btVector3 objectPos;
    
    // object orientation
    btQuaternion objectOrientation;
    
    // collision shape stype
    string collisionShapeType;
    
    // objcet scaled size
    float scale;
    
    // object mass
    float mass;

		// object bounciness
		float restitution;
    
    // collision box parameters
    btVector3 boxSize;
    
    // collision sphere parameter
    btScalar sphereRadius;
    
    // collision plane parameters
    btVector3 planeDirection;
    btScalar planeConstant;

		// collision capsule parameters
		btScalar capsuleRadius, capsuleHeight;

		// collision cylinder parameters
		btVector3 cylinderSize;
};

#endif // OBJECT_INFO_H
