#version 330 core

out vec4 color;
in vec4 vertexColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;

void main()
{
    color = texture(ourTexture, TexCoord);
	//color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}