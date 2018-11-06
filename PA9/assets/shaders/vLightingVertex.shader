#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texture;
layout (location = 3) in vec3 v_normal;

smooth out vec4 color;

uniform sampler2D sampler;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 lightPos;

void main(void)
{
  vec4 v = vec4(v_position, 1.0);
 
  vec3 pos = (viewMatrix * modelMatrix * vec4(v_position, 0.0)).xyz;

  vec3 nN = normalize((viewMatrix * modelMatrix * vec4(v_normal, 0.0)).xyz);
  vec3 nE = normalize(-pos);
  vec3 nL = normalize(lightPos.xyz - pos);
  vec3 halfVec = normalize(nL + nE);
  
  float Kd = max(dot(nL, nN), 0.0);
  vec4 diffuse = Kd * texture2D(sampler, v_texture);
  
  float Ks = pow(max(dot(nN, halfVec), 0.0), shininess);
  vec4 specular = Ks * specularColor;
  
  if(dot(nL, nN) < 0.0) 
  {
    specular = vec4(0.0, 0.0, 0.0, 1.0);
  }
  
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
  
  color = ambientColor + diffuse + specular;
  color.a = 1.0;
}












