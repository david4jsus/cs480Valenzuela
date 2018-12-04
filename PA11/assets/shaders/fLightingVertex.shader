#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texture;
layout (location = 2) in vec3 v_normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 firstLightPos;
uniform vec4 secondLightPos;
uniform vec4 thirdLightPos;

smooth out vec2 texture;
smooth out vec3 fN;
smooth out vec3 fE;
smooth out vec3 fLFirstLight;
smooth out vec3 fLSecondLight;
smooth out vec3 fLThirdLight;

void main(void)
{
   vec4 vPos = vec4(v_position, 1.0);
   
   fN = mat3(transpose(inverse(modelMatrix))) * v_normal;
   fE = vec3(modelMatrix * vPos);
   fLFirstLight = firstLightPos.xyz - fE;
	 fLSecondLight = secondLightPos.xyz - fE;
	 fLThirdLight = thirdLightPos.xyz - fE;

   texture = v_texture;

   gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vPos;
}
