#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include "graphics_headers.h"

<<<<<<< HEAD
=======
#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh 
>>>>>>> master

class Object
{
  public:
    Object();
    Object(const std::string filePath, Object* objParent, float objOrbitRadius, float objOrbitMultiplier,
      float objRotateMultiplier, float objSize);
    ~Object();
    void Update(unsigned int dt);
    void Render();

    void loadOBJ(const std::string& pFile);

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
    std::vector<std::vector<Vertex>> Vertices;
    std::vector<std::vector<unsigned int>> Indices;
    std::vector<GLuint> VB;
    std::vector<GLuint> IB;
    
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
    
    // Object loading
    std::string objFilePath;
    bool correctModelLoad;
    std::vector<Vertex> myVertices;
    std::vector<unsigned int> myIndices;
<<<<<<< HEAD
=======
    bool loadOBJ(std::string path, std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices);

	// Assimp object loader
	Assimp::Importer importer;
	const aiScene *scene;
	std::vector<aiMesh*> meshes;
	
>>>>>>> master
};

#endif /* OBJECT_H */
