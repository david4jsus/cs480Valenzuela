#version 330

smooth in vec3 color;
smooth in vec2 texture;

smooth in vec3 fN;
smooth in vec3 fL;
smooth in vec3 fE;
smooth in vec3 sFL;
smooth in vec3 spotlight_position;

out vec4 frag_color;

uniform sampler2D sampler;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

// Spot Light
uniform vec4 spotAmbience;
uniform vec4 lightDirection;
uniform float clipDistance;

vec4 GetLight(vec3 incoming, vec3 halfway, vec3 normal);

vec4 GetSpotLight(vec3 incoming, vec3 halfway, vec3 normal);

void main(void)
{
   vec4 finalLight;

   vec3 nN = normalize(fN);
   vec3 nE = normalize(-fE);
   vec3 nL = normalize(fL);
   vec3 nSL = normalize(sFL);
   vec3 halfVec = normalize(nL + nE);

   finalLight = GetLight(nL, halfVec, nN) + GetSpotLight(nSL, normalize(sFL + spotlight_position), nN);

   frag_color = finalLight * texture2D(sampler, texture.xy);

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
  
   ks = pow(max(dot(normal, halfway), 0.0), shininess);

   specular = ks * diffuseColor;

   if(dot(incoming, normal) < 0.0)
   {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
   }

   retColor = ambient + diffuse + specular;
   retColor.a = 1.0;

   return retColor;
}

vec4 GetSpotLight(vec3 incoming, vec3 halfway, vec3 normal)
{
   vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
   float angle = 1.0;
   vec3 lightDir;
   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
   float ks;
   float kd;

   float intensity;
   float brightnessMultiplier;

   lightDir = normalize(sFL.xyz - spotlight_position.xyz);

   angle = dot(lightDir, normalize(-lightDir));

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

       intensity = max(dot(normal, lightDir), 0.0);

       brightnessMultiplier = max(max(max(ambient.r, ambient.g), ambient.b), 0.0);

       finalColor = brightnessMultiplier * max(intensity * diffuse + specular, ambient);
   }

    return finalColor;
}


















