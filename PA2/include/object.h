#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();
    
    // To reverse direction of rotation based on keyboard input
    void reverseDirection();
    // Toggle cube rotating (self-centered)
    void toggleRotation();
    // Toggle cube orbiting
    void toggleOrbit();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float angle;
    float rotAngle;
    
    // To reverse direction of rotation based on keyboard input
    bool directionReversed;
    // To rotate in place
    bool rotating;
    // To orbit around
    bool orbiting;
};

#endif /* OBJECT_H */
