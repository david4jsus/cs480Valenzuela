#version 330

<<<<<<< HEAD
smooth in vec3 color;
smooth in vec2 texture;
uniform sampler2D sampler;
=======


smooth in vec3 color;
>>>>>>> master

out vec4 frag_color;

void main(void)
{
<<<<<<< HEAD
   frag_color = texture2D(sampler, texture);
=======
   frag_color = vec4(color.rgb, 1.0);
>>>>>>> master
}
