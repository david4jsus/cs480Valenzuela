#version 330

smooth in vec2 texture;
smooth in vec3 fN;
smooth in vec3 fL;
smooth in vec3 fE;

out vec4 frag_color;

uniform sampler2D sampler;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

void main(void)
{
   vec3 nN = normalize(fN);
   vec3 nE = normalize(fE);
   vec3 nL = normalize(fL);
   vec3 halfVec = normalize(nL + nE);
   
   float Kd = max(dot(nL, nN), 0.0);
   vec4 diffuse = Kd * texture2D(sampler, texture);
   
   float Ks = pow(max(dot(nN, halfVec), 0.0), shininess);
   vec4 specular = Ks * specularColor;
   
   if( dot(nL, nN) < 0.0 )
   {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
   }
   
   frag_color = ambientColor + texture2D(sampler, texture) + specularColor;
   frag_color.a = 1.0;
}
