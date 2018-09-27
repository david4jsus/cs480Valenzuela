#include "object.h"

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
  
  parent = 0;
  orbitRadius = 5.0f;
  orbitSpeedMultiplier = 1.0f;
  rotateSpeedMultiplier = 1.0f;
  size = 1;
}

Object::Object(std::string filePath, Object* objParent, float objOrbitRadius, float objOrbitMultiplier,
  float objRotateMultiplier, float objSize): Object()
{
  objFilePath = filePath;
  parent = objParent;
  orbitRadius = objOrbitRadius;
  orbitSpeedMultiplier = objOrbitMultiplier;
  rotateSpeedMultiplier = objRotateMultiplier;
  size = objSize;
  
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
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
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
  
  if (parent == 0)
  {
    center = glm::mat4(1.0f);
  }
  else
  {
    center = parent->GetModelForChild();
  }
  
  // Calculate orbit rotation only if orbiting
  if (orbiting)
  {
    // Reverse direction of rotation bsed on input
    if (directionReversed)
    {
      angle -= dt * orbitSpeedMultiplier * M_PI/1000;
    }
    else
    {
      angle += dt * orbitSpeedMultiplier * M_PI/1000;
    }
  }

  // Orbit rotation
  model = glm::translate(center, glm::vec3(glm::sin(angle) * orbitRadius,
    0, glm::cos(angle) * orbitRadius));
  
  // Pass this to any children objects
  modelForChild = model;
  
  // Self-centered rotation
  if (rotating)
  {
    if (directionReversed)
    {
      rotAngle -= dt * rotateSpeedMultiplier * M_PI/1000;
    }
    else
    {
      rotAngle += dt * rotateSpeedMultiplier * M_PI/1000;
    }
  }
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

// To reverse the direction of rotation based on keyboard input
void Object::reverseDirection()
{
  directionReversed = !directionReversed;
}

// To make direction counter-clockwise
void Object::makeDirectionCounter()
{
  directionReversed = false;
}

// To make direction clockwise
void Object::makeDirectionClockwise()
{
  directionReversed = true;
}

// Toggle cube rotating (self-centered)
void Object::toggleRotation()
{
  rotating = !rotating;
}

// Toggle cube orbiting
void Object::toggleOrbit()
{
  orbiting = !orbiting;
}

bool Object::isDirectionReversed()
{
  return directionReversed;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  if (correctModelLoad)
  {
    glDrawElements(GL_TRIANGLES, myIndices.size(), GL_UNSIGNED_INT, 0);
  }
  else
  {
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

bool Object::loadOBJ(std::string path, std::vector<Vertex> &out_vertices,
  std::vector<unsigned int> &out_indices)
{
  std::string completeFilePath = "../assets/models/" + path;
  char filePath[completeFilePath.length() + 1];
  strcpy(filePath, completeFilePath.c_str());
  
  // Open file
  FILE *file = fopen(filePath, "r");
  if (file == NULL)
  {
    std::cout << "ERROR: Unable to open file!" << std::endl;
    return false;
  }
  
  // Read file until the end
  while (true)
  {
    char lineHeader[128];
    
    // Read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF) // End of file reached
    {
      break;
    }
    
    // Parse object info
    if (strcmp(lineHeader, "v") == 0) // Vertices
    {
      glm::vec3 vertex;
      glm::vec3 color;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      color.x = glm::sin(vertex.x);
      color.y = glm::sin(vertex.y);
      color.z = glm::sin(vertex.z);
      out_vertices.push_back(Vertex(vertex, color));
    }
    else if (strcmp(lineHeader, "f") == 0) // Faces
    {
      unsigned int vertexIndex[3];
      fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
      out_indices.push_back(vertexIndex[0]);
      out_indices.push_back(vertexIndex[1]);
      out_indices.push_back(vertexIndex[2]);
    }
  }
  fclose(file);
  
  // Correct indices
  for (unsigned int i = 0; i < out_indices.size(); i++)
  {
    out_indices[i] = out_indices[i] - 1;
  }
  
  return true;
}
