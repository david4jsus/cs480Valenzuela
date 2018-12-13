#version 330

smooth in vec2 texture;
smooth in vec3 fN;
smooth in vec3 fL;
smooth in vec3 fE;

uniform sampler2D sampler;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

out vec4 frag_color;

void main(void)
{
   vec3 nN = normalize(fN);
   vec3 nE = normalize(-fE);
   vec3 nL = normalize(fL);
   vec3 halfVec = normalize(nL + nE);
   
   float Kd = max(dot(nL, nN), 0.0);
   if (Kd > 0.5)
   {
      Kd = 1;
   }
   else if (Kd > 0.25)
   {
      Kd = 0.5;
   }
   else
   {
      Kd = 0;
   }
   vec4 diffuse = Kd * diffuseColor;
   
   float Ks = pow(max(dot(nN, halfVec), 0.0), shininess);
   if (Ks > 0.5)
   {
      Ks = 1;
   }
   else if (Ks > 0.25)
   {
      Ks = 0.5;
   }
   else
   {
      Ks = 0;
   }
   vec4 specular = Ks * specularColor;
   
   if( dot(nL, nN) < 0.0 )
   {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
   }
   
   frag_color = (ambientColor + diffuse + specular) * texture2D(sampler, texture);
   frag_color.a = 1.0;
}
