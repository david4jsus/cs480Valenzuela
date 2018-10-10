#include "object.h"
<<<<<<< HEAD
#include <fstream>
#include <algorithm>

#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh objects 
=======
#include <iostream>

using namespace std;
>>>>>>> master

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

Object::Object(const std::string filePath, Object* objParent, float objOrbitRadius, float objOrbitMultiplier,
  float objRotateMultiplier, float objSize): Object()
{
  parent = objParent;
  orbitRadius = objOrbitRadius;
  orbitSpeedMultiplier = objOrbitMultiplier;
  rotateSpeedMultiplier = objRotateMultiplier;
  size = objSize;
  
  loadOBJ(filePath);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
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
/*
    unsigned int index;

    for( index = 0; index < std::min( VB.size(), IB.size() ); index++ )
    {
        glEnableVertexAttribArray( 0 );
        glEnableVertexAttribArray( 1 );

<<<<<<< HEAD
        glBindBuffer( GL_ARRAY_BUFFER, VB[ index ] );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), 0 );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* ) offsetof( Vertex, color ) );
=======

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
>>>>>>> master

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IB[ index ] );
        glDrawElements( GL_TRIANGLES, Indices[ index ].size( ), GL_UNSIGNED_INT, 0 );

        glDisableVertexAttribArray( 0 );
        glDisableVertexAttribArray( 1 );
    }
*/
}

void Object::loadOBJ(const std::string& pFile)
{
<<<<<<< HEAD
    Assimp::Importer importer;
Vertex tmpVert( glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ) );
    const aiScene* scene = importer.ReadFile(pFile, aiProcess_Triangulate);

aiMaterial* mtlPtr = NULL;
aiColor4D mColor;

    
    unsigned int mIndex, fIndex, vIndex, iIndex;

    if( scene == NULL )
    {
        std::cout << "Failed to load " << pFile << std::endl;
    }

    Vertices.clear( );
    VB.clear( );
    Indices.clear( );
    IB.clear( );
/*
    for( mIndex = 0; mIndex < scene->mNumMeshes; mIndex++ )
    {
        mtlPtr = scene->mMaterials[ scene->mMeshes[ mIndex ]->mMaterialIndex ];

        if( mtlPtr != NULL )
        {
            if( AI_SUCCESS == aiGetMaterialColor( mtlPtr, AI_MATKEY_COLOR_DIFFUSE, &mColor ) )
            {
                tmpVert.color.r = mColor.r;
                tmpVert.color.g = mColor.g;
                tmpVert.color.b = mColor.b;
            }
        }        

        Vertices.push_back( std::vector<Vertex>( ) );
        Indices.push_back( std::vector<unsigned int>( ) );

        for( fIndex = 0; fIndex < scene->mMeshes[mIndex]->mNumFaces; fIndex++ )
        {
            for( iIndex = 0; iIndex < scene->mMeshes[mIndex ]->mFaces[fIndex].mNumIndices; iIndex++ )
            {
                vIndex = scene->mMeshes[ mIndex ]->mFaces[ fIndex ].mIndices[ iIndex ];
                tmpVert.vertex.x = scene->mMeshes[ mIndex ]->mVertices[ vIndex ].x;
                tmpVert.vertex.y = scene->mMeshes[ mIndex ]->mVertices[ vIndex ].y;
                tmpVert.vertex.z = scene->mMeshes[ mIndex ]->mVertices[ vIndex ].z;                

                Vertices[ mIndex ].push_back( tmpVert );
                Indices[ mIndex ].push_back( scene->mMeshes[ mIndex ]->mFaces[fIndex].mIndices[ iIndex ] );

            }
        }
    }

    VB.resize( Vertices.size( ) );
    IB.resize( Indices.size( ) );
    
    for( vIndex = 0; vIndex < VB.size( ); vIndex++ )
    {
        glGenBuffers( 1, &VB[ vIndex ] );
        glBindBuffer( GL_ARRAY_BUFFER, VB[ vIndex ] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * Vertices[ vIndex ].size( ), &Vertices[ vIndex ][ 0 ], GL_STATIC_DRAW );
    }
    
    for( iIndex = 0; iIndex < IB.size( ); iIndex++ )
    {
        glGenBuffers( 1, &IB[ iIndex ] );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IB[ iIndex ] );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * Indices[ iIndex ].size( ), &Indices[ iIndex ][ 0 ], GL_STATIC_DRAW );
    }
*/
=======
  // local variables
  unsigned int meshCounter, faceLooper, verticesLooper, indicesLooper;
  unsigned int lastValue = 0;
  glm::vec3 vertex;
  glm::vec3 color;

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
  }

  // loop through all meshes
  for(meshCounter = 0; meshCounter < scene->mNumMeshes; meshCounter++)
	{
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
		  // get x, y, and z coordinates for each vertex
		  vertex.x = meshes[meshCounter]->mVertices[verticesLooper].x;
		  vertex.y = meshes[meshCounter]->mVertices[verticesLooper].y;
		  vertex.z = meshes[meshCounter]->mVertices[verticesLooper].z;

		  // assign color to a vertex
		  color.x = glm::sin(vertex.x);
		  color.y = glm::sin(vertex.y);
		  color.z = glm::sin(vertex.z);

          // store vertexes
		  Vertex batmanVertices(vertex, color);
		  out_vertices.push_back(batmanVertices);
		}
	  }

  // object file sucessfully accessed
  return true;
>>>>>>> master
}
