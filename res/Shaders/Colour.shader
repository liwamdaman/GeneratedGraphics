#shader vertex
#version 330 core

layout(location = 0) in vec2 position;	// the position variable has attribute position 0
layout(location = 1) in vec4 color;		// the colour variable has attribute position 1

out vec4 ourColor; // output a colour to the fragment shader

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	ourColor = color;
};

#shader fragment
#version 330 core

//layout(location = 0) out vec4 fragColor;
out vec4 fragColor;
in vec4 ourColor;

void main()
{
	fragColor = ourColor;
};