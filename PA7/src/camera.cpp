#include "camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

bool Camera::Initialize(int w, int h)
{
  yaw = pitch = 0.0f;
  
  //glm::vec3 target, direction;
  
  cameraPos   = glm::vec3(-20.0f, 5.0f, 0.0f);
  cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
  
  view = glm::lookAt(cameraPos,               //Eye Position
                     cameraPos + cameraFront, //Focus point
                     cameraUp);               //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

void Camera::updateCamPosXPos(float deltax)
{
   cameraPos += deltax * cameraFront;
   updateViewMatrix();
}

void Camera::updateCamPosXNeg(float deltax)
{
   cameraPos -= deltax * cameraFront;
   updateViewMatrix();
}

void Camera::updateCamPosYPos(float deltay)
{
   cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
   cameraPos += deltay * cameraRight;
   updateViewMatrix();
}

void Camera::updateCamPosYNeg(float deltay)
{
   cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
   cameraPos -= deltay * cameraRight;
   updateViewMatrix();
}

void Camera::updateCamPosZPos(float deltaz)
{
   cameraPos += deltaz * cameraUp;
   updateViewMatrix();
}

void Camera::updateCamPosZNeg(float deltaz)
{
   cameraPos -= deltaz * cameraUp;
   updateViewMatrix();
}

void Camera::updateCamRotYaw(float deltayaw)
{
   yaw += deltayaw;
   updateViewMatrix();
}

void Camera::updateCamRotPitch(float deltapitch)
{
   pitch += deltapitch;
   
   if (pitch > 89.0f)
   {
      pitch = 89.0f;
   }
   if (pitch < -89.0f)
   {
      pitch = -89.0f;
   }
   
   updateViewMatrix();
}

void Camera::updateViewMatrix()
{
   glm::vec3 front;
   front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   front.y = sin(glm::radians(pitch));
   front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   cameraFront = glm::normalize(front);
   
   view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

