#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D image;
uniform vec3 image_color;

void main()
{
    FragColor = vec4(image_color, 1.0) * texture(image, TexCoord);
}