#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    Object(Object* objParent, float objOrbitRadius, float objOrbitMultiplier,
      float objRotateMultiplier, float objSize);
    ~Object();
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();
    glm::mat4 GetModelForChild();
    
    // To reverse direction of rotation
    void reverseDirection();
    // To make direction counter-clockwise
    void makeDirectionCounter();
    // To make direction clockwise
    void makeDirectionClockwise();
    // Toggle cube rotating (self-centered)
    void toggleRotation();
    // Toggle cube orbiting
    void toggleOrbit();
    
    // Check if reverse direction for orbit
    bool isDirectionReversed();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    
    // If the object has a child, this is the matrix it should read as its center
    glm::mat4 modelForChild;

    float angle;
    float rotAngle;
    
    // To reverse direction of rotation based on keyboard input
    bool directionReversed;
    // To rotate in place
    bool rotating;
    // To orbit around
    bool orbiting;
    
    
    // Planet/moon properties
    Object* parent;
    float orbitRadius;
    float orbitSpeedMultiplier;
    float rotateSpeedMultiplier;
    float size;
};

#endif /* OBJECT_H */
