#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texture;
layout (location = 3) in vec3 v_normal;

smooth out vec2 texture;
smooth out vec3 fN;
smooth out vec3 fE;
smooth out vec3 fL;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 lightPos;

void main(void)
{
   fN = v_normal;
   fE = v_position;
   fL = lightPos.xyz;
   
   texture = v_texture;

   vec4 vPos = vec4(v_position, 1.0);

   if(lightPos.w != 0.0)
   {
       fL = lightPos.xyz - vPos.xyz;
   }

   gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vPos;
}
