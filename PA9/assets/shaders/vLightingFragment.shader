#version 330

smooth in vec4 color;
smooth in vec2 texture;

uniform sampler2D sampler;

out vec4 frag_color;

void main(void)
{
   frag_color = color * texture2D(sampler, texture);
}
