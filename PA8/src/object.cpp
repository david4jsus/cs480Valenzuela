#include "object.h"
#include <iostream>
#include <fstream>

using namespace std;

Object::Object()
{
  // To reverse direction of rotation based on keyboard input
  directionReversed = false;
  // To rotate in place
  rotating = true;
  // To orbit around
  orbiting = true;

  angle = 0.0f;
  rotAngle = 0.0f;
  
  objName = "";
  parent = 0;
  orbitRadius = 5.0f;
  orbitSpeedMultiplier = 1.0f;
  rotateSpeedMultiplier = 1.0f;
  rsm = rotateSpeedMultiplier;
  osm = orbitSpeedMultiplier;
  size = 1;
}

Object::Object(std::string filePath, Object* objParent, float objOrbitRadius, float objOrbitMultiplier,
  float objRotateMultiplier, float objSize): Object()
{
  // local variables
  //objName.erase(objName.end()-4, objName.end());

  objFilePath = filePath;
  parent = objParent;
  orbitRadius = objOrbitRadius;
  orbitSpeedMultiplier = objOrbitMultiplier;
  rotateSpeedMultiplier = objRotateMultiplier;
  size = objSize;
  
  // create object
  createObject();
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::createObject()
{
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */
  
    Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}

  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }
  
  // Load model
  if (objFilePath == "")
  {
    correctModelLoad = false;
    std::cout << "Loading default cube object..." << std::endl;
    objName = "Cube";
  }
  else
  {
    correctModelLoad = loadOBJ(objFilePath, myVertices, myIndices);
  }
  
  if (correctModelLoad) // If the object loads
  {
    std::cout << "Loading " << objFilePath << "..." << std::endl;
    
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * myVertices.size(), &myVertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * myIndices.size(),
      &myIndices[0], GL_STATIC_DRAW);
  }
  else // If the object fails to load, load a cube
  {
    //std::cout << "ERROR: Model could not be loaded. Loading default cube object..." << std::endl;
    
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
  }
}

void Object::Update(unsigned int dt)
{
  // Determine center
  glm::mat4 center;

  model = glm::translate(center, pos);
  
  // Pass this to any children objects
  modelForChild = model;
  
  model = glm::rotate(model, (rotAngle), glm::vec3(0.0, 1.0, 0.0));
  
  // Scaling
  model = glm::scale(model, glm::vec3(size, size, size));
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetModelForChild()
{
  return modelForChild;
}

glm::vec3 Object::objectPosition()
{
   glm::vec3 objPos = pos;
   
   if (parent != 0)
   {
      objPos += parent->objectPosition();
   }
   
   return objPos;
}

void Object::setPosition(glm::vec3 newPos)
{
   pos = newPos;
}

std::string Object::GetObjectName()
{
   return objName;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  if (correctModelLoad)
  {
    // Bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    
    // Draw
    glDrawElements(GL_TRIANGLES, myIndices.size(), GL_UNSIGNED_INT, 0);
  }
  else
  {
    // Bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    
    // Draw
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

bool Object::loadOBJ(std::string path, std::vector<Vertex> &out_vertices,
  std::vector<unsigned int> &out_indices)
{
  // local variables
  unsigned int meshCounter, faceLooper, verticesLooper, indicesLooper;
  unsigned int lastValue = 0;
  glm::vec3 vertex;
  glm::vec3 color;
  glm::vec2 texture;

  // string that contains path to object file
  std::string completeFilePath = "../assets/models/" + path;

  // access information from object file
  scene = importer.ReadFile(completeFilePath, aiProcess_Triangulate);

  // check if object file was sucessfully accessed
  if(scene == NULL)
  {
    std::cout << "ERROR: Unable to open file!" << std::endl;
    return false;
  }

  // get all meshes from object scene
  for(meshCounter = 0; meshCounter < scene->mNumMeshes; meshCounter++)
  {
    meshes.push_back(scene->mMeshes[meshCounter]);
        
    // Check if the model has a texture
    meshes[meshCounter]->HasTextureCoords(0);
    //cout << "has texture" << endl;
    
     // loop through all faces
	  for(faceLooper = 0; faceLooper < meshes[meshCounter]->mNumFaces; faceLooper++)
	  {
		// loop through all indices
	    for(indicesLooper = 0; indicesLooper < 3; indicesLooper++)
		{
          // get position of index
		  out_indices.push_back(meshes[meshCounter]->mFaces[faceLooper].mIndices[indicesLooper] + lastValue);
		}
	  }

      // offest new next mesh's index poisition
	  lastValue = out_indices[out_indices.size() - 1] + 1;
			  
	  // loop through all vertexes
	  for(verticesLooper = 0; verticesLooper < meshes[meshCounter]->mNumVertices; verticesLooper++)
		{
		  // Texture coordinates
          aiVector3D vert = meshes[meshCounter]->mTextureCoords[0][verticesLooper];
          texture.x = vert.x;
          texture.y = vert.y * -1;
		
		  // get x, y, and z coordinates for each vertex
		  vertex.x = meshes[meshCounter]->mVertices[verticesLooper].x;
		  vertex.y = meshes[meshCounter]->mVertices[verticesLooper].y;
		  vertex.z = meshes[meshCounter]->mVertices[verticesLooper].z;

		  // assign color to a vertex
		  color.x = glm::sin(vertex.x);
		  color.y = glm::sin(vertex.y);
		  color.z = glm::sin(vertex.z);

          // store vertexes
		  Vertex batmanVertices(vertex, color, texture);
		  out_vertices.push_back(batmanVertices);
		}
		
	    // Read texture file
	    aiString assimpFilePath;
	    string imageFilePath;
	    scene->mMaterials[scene->mMeshes[meshCounter]->mMaterialIndex]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), assimpFilePath);
	    imageFilePath = assimpFilePath.C_Str();
	    imageFilePath = "../assets/images/" + imageFilePath;
		
		  // Load Texture
      Magick::Blob blob;
      Magick::Image *image;
      image = new Magick::Image(imageFilePath);
      image->write(&blob, "RGBA");
      //cout << "Loaded Texture: " << assimpFilePath.C_Str() << endl;

      // Generate Texture
      glGenTextures(1, &Texture);
      glBindTexture(GL_TEXTURE_2D, Texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      delete image;
      //cout << "Generated Texture" << endl;
	  }

  // object file sucessfully accessed
  return true;
}
