#define GLEW_STATIC

#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <Shader/Shader.h>


GLfloat triangle01[] = {
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  
};

GLfloat rectangle01[] = {
	 0.5f,  0.5f, 0.0f, 
	 0.5f, -0.5f, 0.0f, 
	-0.5f, -0.5f, 0.0f, 
	-0.5f,  0.5f, 0.0f  
};

GLuint indices[] = { 
	0, 1, 3, 
	1, 2, 3
};


int main() {

	int width, height;

	double prevSecond = glfwGetTime();
	double currSecond;
	int frameCount = 0;

	// GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Window
	GLFWwindow* window = glfwCreateWindow(600, 600, "Beautiful scene", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Success Check
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Shader
	Shader ourShader("shader.vs", "shader.frag");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	// Vertex Buffer Object, Vertex Array Object, Element Buffer Object
	// Initialization
	GLuint VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);

	// -------------------------------------------------------------------------------------------------------------------- //
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle01), triangle01, GL_STATIC_DRAW);
	// object 2
	// object 3
	// object ...




	// -------------------------------------------------------------------------------------------------------------------- //

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Using key callbacks
	void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
	glfwSetKeyCallback(window, KeyCallback);



	// Game Loop
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.Use();
		//GLfloat timeValue = glfwGetTime();
		//GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		//GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);



		glfwSwapBuffers(window);

		// FPS Counter
		currSecond = glfwGetTime();
		frameCount++;
		if (currSecond - prevSecond >= 1.0) {
			std::cout << "FPS: " << frameCount << std::endl;
			frameCount = 0;
			prevSecond += 1.0;
		}
	}

	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}




}