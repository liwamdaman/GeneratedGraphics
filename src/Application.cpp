#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "FractalTree.h"
#include "Maze.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(DEFAULT_NATIVE_RESOLUTION_WIDTH, DEFAULT_NATIVE_RESOLUTION_HEIGHT, "Look at dis shit", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error initializing glew" << std::endl;

	GLCall(std::cout << glGetString(GL_VERSION) << std::endl);
	{
		/* Test code */
		//float positions[] = {
		//	-1,	-1,
		//	 1,	-1,
		//	 1,	 1,
		//	-1,	 1,
		//	/*0.0,	0.0,
		//	0.01,	0.01,
		//	0.51,	0.01,
		//	0.5,	0.0*/
		//};

		//unsigned int indices[] = {
		//	0, 1, 2,
		//	2, 3, 0
		//};

		//VertexArray va;
		//VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		//VertexBufferLayout layout;
		//layout.Push<float>(2);
		//va.AddBuffer(vb, layout);

		//IndexBuffer ib(indices, 6);

		//Shader shader("res/shaders/Basic.shader");
		//shader.Bind();
		//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Renderer renderer;

		//FractalTree fTree = FractalTree(10, 2);
		Maze maze = Maze(16,9,0,0);

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			//renderer.Draw(va, ib, shader);

			// The Goal:
			//wait for some input or automatically start on first loop
			maze.Iterate();
			renderer.Draw(&maze);

			//fTree.GetShader().Bind();
			//fTree.GetShader().SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			//renderer.Draw(&fTree);

			if (r > 1.0f)
				increment = -0.05f;
			if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

			//std::cin.ignore();
		}
	}

	glfwTerminate();
	return 0;
}