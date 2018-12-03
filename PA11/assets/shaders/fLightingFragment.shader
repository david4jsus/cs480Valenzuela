#version 330

smooth in vec2 texture;
smooth in vec3 fN;
smooth in vec3 fL;
smooth in vec3 fE;
smooth in vec3 fL1;

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

	 vec4 diffuseColor2 = vec4(0, 0, 1, 1);
   
   float Kd = max(dot(nL, nN), 0.0);
   vec4 diffuse = Kd * diffuseColor2;
   
   float Ks = pow(max(dot(nN, halfVec), 0.0), shininess);
   vec4 specular = Ks * specularColor;
   
   if( dot(nL, nN) < 0.0 )
   {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
   }




	 vec3 nN1 = normalize(fN);
   vec3 nE1 = normalize(-fE);
   vec3 nL1 = normalize(fL1);
   vec3 halfVec1 = normalize(nL1 + nE1);

	 vec4 diffuseColor1 = vec4(1, 0, 0, 1);
   
   float Kd1 = max(dot(nL1, nN1), 0.0);
   vec4 diffuse1 = Kd1 * diffuseColor1;
   
   float Ks1 = pow(max(dot(nN1, halfVec1), 0.0), shininess);
   vec4 specular1 = Ks1 * specularColor;
   
   if( dot(nL1, nN1) < 0.0 )
   {
      specular1 = vec4(0.0, 0.0, 0.0, 1.0);
   }

		




   
   frag_color = ((ambientColor + diffuse + specular) + (ambientColor + diffuse1 + specular1)) * texture2D(sampler, texture);
   frag_color.a = 1.0;
}
