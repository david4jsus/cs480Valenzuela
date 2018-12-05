#version 330

smooth in vec2 texture;
smooth in vec3 fN;
smooth in vec3 fE;
smooth in vec3 fLFirstLight;
smooth in vec3 fLSecondLight;
smooth in vec3 fLThirdLight;

uniform sampler2D sampler;
uniform vec4 ambientColor;
uniform vec4 diffuseColorFirstLight;
uniform vec4 specularColorFirstLight;
uniform float shininessFirstLight;

uniform vec4 diffuseColorSecondLight;
uniform vec4 specularColorSecondLight;
uniform float shininessSecondLight;

/*uniform vec4 diffuseColorThirdLight;
uniform vec4 specularColorThirdLight;
uniform float shininessThirdLight;*/

out vec4 frag_color;

void main(void)
{
   vec3 nN = normalize(fN);
   vec3 nE = normalize(-fE);
   vec3 nLFirstLight = normalize(fLFirstLight);
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


   vec3 nLSecondLight = normalize(fLSecondLight);
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


	 /*vec3 nLThirdLight = normalize(fLThirdLight);
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




   
   frag_color = ((ambientColor + diffuseFirstLight + specularFirstLight) + (diffuseSecondLight + specularSecondLight)) * texture2D(sampler, texture);
   frag_color.a = 1.0;
}
