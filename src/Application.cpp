#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "FractalTree.h"
#include "Maze.h"
#include "CyclicCA.h"

void testBasic(GLFWwindow* window);
void testFractalTree(GLFWwindow* window);
void testMazeGeneration(GLFWwindow* window);
void testCyclicCellularAutomation(GLFWwindow* window);

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
	window = glfwCreateWindow(DEFAULT_NATIVE_RESOLUTION_WIDTH, DEFAULT_NATIVE_RESOLUTION_HEIGHT, "Look at dis stuff", NULL, NULL);
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

	/* CALL TEST HERE */
	//testBasic(window);
	//testFractalTree(window);
	//testMazeGeneration(window);
	testCyclicCellularAutomation(window);


	glfwTerminate();
	return 0;
}

void testBasic(GLFWwindow *window) {
	float positions[] = {
		//-1,	-1,
		// 1,	-1,
		// 1,	 1,
		//-1,	 1,
		0.0,	0.0,
		0.01,	0.01,
		0.51,	0.01,
		0.5,	0.0
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray va;
	VertexBuffer vb(positions, 4 * 2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	Renderer renderer;

	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		renderer.Draw(va, ib, shader);

		if (r > 1.0f)
			increment = -0.05f;
		if (r < 0.0f)
			increment = 0.05f;

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}

void testFractalTree(GLFWwindow* window) {
	
	Renderer renderer;
	FractalTree fTree = FractalTree(10, 2);

	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		fTree.GetShader().Bind();
		fTree.GetShader().SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
		renderer.Draw(&fTree);

		if (r > 1.0f)
			increment = -0.05f;
		if (r < 0.0f)
			increment = 0.05f;

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}

void testMazeGeneration(GLFWwindow* window) {

	Renderer renderer;
	Maze maze = Maze(16, 9, 0, 5, true);
	//Maze maze = Maze(48, 27, 10, 10, false);
	//Maze maze = Maze(64, 36, 16, 18, false);
	
	//Seed random generator
	srand(time(NULL));

	auto start = std::chrono::high_resolution_clock::now();
	const std::chrono::milliseconds iterationTimePeriod(10);
	bool mazeGenStarted = false;
	bool mazGenFinished = false;
	int i = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		if (!mazGenFinished && 
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start) > iterationTimePeriod) {
			// Reset start time
			start = std::chrono::high_resolution_clock::now();

			// Iterate maze generation
			i = maze.Iterate();
			if (!mazeGenStarted) mazeGenStarted = true;
			if (i == 1) mazGenFinished = true;
		}
		
		if(mazeGenStarted)
			renderer.Draw(&maze);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}

void testCyclicCellularAutomation(GLFWwindow* window) {

	Renderer renderer;
	CyclicCA rule313 = CyclicCA();	// Default constructor builds rule 313 by David Griffeath

	//Seed random generator
	srand(time(NULL));

	auto start = std::chrono::high_resolution_clock::now();
	const std::chrono::milliseconds iterationTimePeriod(100);
	bool mazeGenStarted = false;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start) > iterationTimePeriod) {
			// Reset start time
			start = std::chrono::high_resolution_clock::now();

			// Iterate maze generation
			rule313.Iterate();
			if (!mazeGenStarted) mazeGenStarted = true;
		}

		if (mazeGenStarted)
			renderer.Draw(&rule313);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}