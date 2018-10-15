#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    
    void updateCamPosXPos (float deltax);
    void updateCamPosXNeg (float deltax);
    void updateCamPosYPos (float deltay);
    void updateCamPosYNeg (float deltay);
    void updateCamPosZPos (float deltaz);
    void updateCamPosZNeg (float deltaz);
    void updateCamRotYaw  (float deltayaw);
    void updateCamRotPitch(float deltapitch);
  
  private:
    void updateViewMatrix();
    
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;     // Positive X-axis
    glm::vec3 cameraRight;     // positive Y-axis
    glm::vec3 cameraUp;        // positive Z-axis
    float yaw;
    float pitch;
};

#endif /* CAMERA_H */

