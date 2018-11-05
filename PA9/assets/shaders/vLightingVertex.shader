#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture;

smooth out vec3 color;
smooth out vec2 texture;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

// Light Info
uniform vec4 lightArr;
uniform vec4 ambientColor;

// Object Info
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

// Spot Light Info
uniform vec4 lightPos;
uniform vec4 spotAmbience;
uniform vec3 lightDir;
uniform float clipDistance;

vec4 GetLight(vec3 incoming, vec3 halfway, vec3 normal);
vec4 GetSpotLight(vec3 incoming, vec3 halfway, vec3 normal, vec4 vPos);

void main(void)
{
  vec4 v = vec4(v_position, 1.0);

  mat4 modelView = viewMatrix * modelMatrix;
 
  vec3 pos = (modelView * vPos).xyz;

  vec3 nN = normalize(modelView * vec4(v_normal, 0.0)).xyz;
  vec3 nE = normalize(-pos);
  vec3 nL = normalize(lightArr.xyz - pos);
  vec3 halfVec = normalize(nL + nE);

  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
  texture = v_texture;

  color = GetLight(nL, halfVec, nN) + GetSpotLight(normalize(lightPos.xyz), normalize(lightPos.xyz + (modelMatrix * v_position).xyz), nN, modelMatrix * v_position);
}

vec4 GetLight(vec3 incoming, vec3 halfway, vec3 normal)
{
   vec4 retColor;
   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
   float ks;
   float kd;

   ambient = ambientColor;

   kd = max(dot(incoming, normal), 0.0);

   diffuse = kd * diffuseColor; 

   ks = pow( max(dot(normal, halfway), 0.0), shininess);

   specular = ks * specularColor;

   if(dot(incoming, normal) < 0.0)
   {
       specular = vec4(0.0, 0.0, 0.0, 1.0);
   }

   retColor = ambient + diffuse + specular;
   retColor.a = 1.0;

   return retColor;
}

vec4 GetSpotLight(vec3 incoming, vec3 halfway, vec3 normal, vec4 vPos)
{
   vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
   float angle = 1.0;
   vec3 lightDirection;
   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
   float ks;
   float kd;

   float intensity;

   float brightnessMultiplier;

   lightDirection = normalize(lightPosition.xyz - vPos.xyz);
    
   angle = dot(lightDirection, normalize(-lightDir));    

   if(angle > clipDistance)
   {       

      ambient = spotAmbience;

      kd = max(dot(incoming, normal), 0.0);

      diffuse = kd * diffuseColor;

      ks = pow(max(dot(normal, halfway), 0.0), shininess);

      specular = ks * specularColor;

      if(dot(incoming, normal) < 0.0)
      {
          specular = vec4(0.0, 0.0, 0.0, 1.0);
      }

      intensity = max(dot(normal, lightDirection), 0.0);

      brightnessMultiplier = max(max(max(ambient.r, ambient.g), ambient.b), 0.0);

      finalColor = brightnessMultiplier * max(intensity * diffuse + specular, ambient);
   }

   return finalColor;   
}










