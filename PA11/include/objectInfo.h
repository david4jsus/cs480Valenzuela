#ifndef OBJECT_INFO_H
#define OBJECT_INFO_H

#include <btBulletDynamicsCommon.h>

struct ObjectInfo
{
  string objectName;
  btVector3 objectPos;
  btQuaternion objectOrientation;
  string collisionShape;
  float scale;
  float mass;
  float xBoxSize, yBoxSize, zBoxSize;
  float sphereRadius;
  float planeDirection, planeConstant;
};

#endif // OBJECT_INFO_H
