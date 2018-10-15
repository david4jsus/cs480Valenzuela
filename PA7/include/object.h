#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh 

class Object
{
  public:
    Object();
    Object(std::string filePath, Object* objParent, float objOrbitRadius, float objOrbitMultiplier,
      float objRotateMultiplier, float objSize);
    ~Object();
    void createObject();
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
    
    // Speed Multipliers
    void UpdateRotationSpeed(float rotateMultiplier);
    void UpdateOrbitSpeed(float orbitMultiplier);
    
    
    // Get Speeds
    float GetRotationSpeed();
    float GetOrbitSpeed();
    
    // Check if reverse direction for orbit
    bool isDirectionReversed();
    
    // Position of an object in space
    glm::vec3 objectPosition();
    
    // Name of object
    std::string GetObjectName();

  private:
    std::string objName;
    glm::mat4 model;
    glm::vec3 pos;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint Texture;
    
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
    
    float osm;
    float rsm;
    
    float size;
    
    // Object loading
    std::string objFilePath;
    bool correctModelLoad;
    std::vector<Vertex> myVertices;
    std::vector<unsigned int> myIndices;
    bool loadOBJ(std::string path, std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices);

	// Assimp object loader
	Assimp::Importer importer;
	const aiScene *scene;
	std::vector<aiMesh*> meshes;
	
	Magick::Image im;
	std::vector<unsigned int> mTextureCoords;
};

#endif /* OBJECT_H */
