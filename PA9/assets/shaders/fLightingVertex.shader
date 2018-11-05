#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texture;
layout (location = 3) in vec2 v_uv;
layout (location = 4) in vec3 v_normal;

smooth out vec3 color;
smooth out vec2 texture;
smooth out vec2 uv;
smooth out vec3 fN;
smooth out vec3 fE;
smooth out vec3 fL;
smooth out vec3 sFL;
smooth out vec3 spotlight_position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec4 lightArr;

uniform vec4 lightPos;

void CalculateLighting(vec4 lPos, vec4 vPos, vec3 normal);

void main(void)
{
   vec4 vPos = vec4(v_position, 1.0);
   mat4 modelView = viewMatrix * modelMatrix;

   mat4 mvp;
   
   vec4 adjustedPos;
   vec4 adjustedNormal;

   mvp = (projectionMatrix * modelView);

   gl_Position = mvp * vPos;

   adjustedPos = (modelView * vPos);
   adjustedNormal = (modelView * vec4(v_normal, 0.0));

   CalculateLighting(lightArr, adjustedPos, adjustedNormal.xyz);

   uv = v_uv;

   spotlight_position = (modelMatrix * vPos).xyz;
}

void CalculateLighting(vec4 lPos, vec4 vPos, vec3 normal)
{
   fN = normal;
   fE = vPos.xyz;
   fL = lPos.xyz;

   if(lPos.w != 0.0)
   {
       fL = lPos.xyz - vPos.xyz;
   }
 
   sFL = lightPos.xyz;
}
