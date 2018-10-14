#version 330

smooth in vec3 color;
smooth in vec2 texture;
uniform sampler2D sampler;
uniform sampler2D sampler1;

out vec4 frag_color;

void main(void)
{
   frag_color = texture2D(sampler, texture);
}
