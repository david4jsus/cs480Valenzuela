#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texture;
layout (location = 2) in vec3 v_normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec4 ambientColor;

uniform vec4 firstLightPos;
uniform vec4 diffuseColorFirstLight;
uniform vec4 specularColorFirstLight;
uniform float shininessFirstLight;

uniform vec4 secondLightPos;
uniform vec4 diffuseColorSecondLight;
uniform vec4 specularColorSecondLight;
uniform float shininessSecondLight;

/*uniform vec4 thirdLightPos;
uniform vec4 diffuseColorThirdLight;
uniform vec4 specularColorThirdLight;
uniform float shininessThirdLight;*/

smooth out vec4 color;
smooth out vec2 texture;

void main(void)
{
  vec4 v = vec4(v_position, 1.0);
 
  vec3 pos = vec3(modelMatrix * vec4(v_position, 1.0));
  vec3 nN = normalize(mat3(transpose(inverse(modelMatrix))) * v_normal);
  vec3 nE = normalize(-pos);



  vec3 nLFirstLight = normalize(firstLightPos.xyz - pos);
  vec3 halfVecFirstLight = normalize(nLFirstLight + nE);

	vec4 diffuseColor2 = vec4(0, 0, 1, 1);
   
  float KdFirstLight = max(dot(nLFirstLight, nN), 0.0);
  vec4 diffuseFirstLight = KdFirstLight * diffuseColor2;
   
  float KsFirstLight = pow(max(dot(nN, halfVecFirstLight), 0.0), shininessFirstLight);
  vec4 specularFirstLight = KsFirstLight * specularColorFirstLight;
   
  if( dot(nLFirstLight, nN) < 0.0 )
  {
     specularFirstLight = vec4(0.0, 0.0, 0.0, 1.0);
  }
  

	vec3 nLSecondLight = normalize(secondLightPos.xyz - pos);
  vec3 halfVecSecondLight = normalize(nLSecondLight + nE);

	vec4 diffuseColor1 = vec4(1, 0, 0, 1);
   
  float KdSecondLight = max(dot(nLSecondLight, nN), 0.0);
  vec4 diffuseSecondLight = KdSecondLight * diffuseColor1;
   
  float KsSecondLight = pow(max(dot(nN, halfVecSecondLight), 0.0), shininessSecondLight);
  vec4 specularSecondLight = KsSecondLight * specularColorSecondLight;
   
  if( dot(nLSecondLight, nN) < 0.0 )
  {
     specularSecondLight = vec4(0.0, 0.0, 0.0, 1.0);
  }


	/*vec3 nLThirdLight = normalize(thirdLightPos.xyz - pos);
  vec3 halfVecThirdLight = normalize(nLThirdLight + nE);

	vec4 diffuseColor1 = vec4(1, 0, 0, 1);
   
  float KdThirdLight = max(dot(nLThirdLight, nN), 0.0);
  vec4 diffuseThirdLight = KdThirdLight * diffuseColor1;
   
  float KsThirdLight = pow(max(dot(nN, halfVecThirdLight), 0.0), shininessThirdLight);
  vec4 specularThirdLight = KsThirdLight * specularColorThirdLight;
   
  if( dot(nLThirdLight, nN) < 0.0 )
  {
     specularThirdLight = vec4(0.0, 0.0, 0.0, 1.0);
  }*/

  
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
  
  texture = v_texture;
  
  color = (ambientColor + diffuseFirstLight + specularFirstLight) + (diffuseSecondLight + specularSecondLight);
  color.a = 1.0;
}












