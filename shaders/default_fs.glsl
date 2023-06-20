#version 460 core

out vec4 FragColor;

in vec3 myColor;
in vec2 myTex;

uniform sampler2D texture0;

void main()
{
	FragColor = texture(texture0, myTex) * vec4(myColor, 1.0);
	//FragColor = vec4(myColor, 1.0);
}