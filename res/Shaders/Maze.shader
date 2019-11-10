#shader vertex
#version 330 core

layout(location = 0) in vec4 position;	// the position variable has attribute position 0
layout(location = 1) in vec4 color;		// the colour variable has attribute position 1

out vec3 ourColor; // output a colour to the fragment shader

void main()
{
	gl_Position = position;
	ourColor = color;
};

#shader fragment
#version 330 core

out vec4 fragColor;
in vec4 ourColor;

void main()
{
	fragColor = ourColor;
};