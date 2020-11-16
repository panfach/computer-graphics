#define GLEW_STATIC
#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <Shader/Shader.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GLfloat triangle01[] = {
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  
};

GLfloat rectangle01[] = {
	// Позиции          // Цвета             // Текстурные координаты
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    
};

GLfloat cube01[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
	glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
	//glfwWindowHint ( GLFW_DEPTH_BITS, 24 );
	//glfwWindowHint ( GLFW_CLEINT_API, GLFW_OPENGL_API );
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

	// GL settings
	glEnable(GL_DEPTH_TEST);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube01), cube01, GL_STATIC_DRAW);
	// object 2
	// object 3
	// object ...




	// -------------------------------------------------------------------------------------------------------------------- //

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Texture 1
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int texWidth, texHeight;
	unsigned char* image = SOIL_load_image("wood.jpg", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Отвязываем
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Texture 2
	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("cat.png", &texWidth, &texHeight, 0, SOIL_LOAD_AUTO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Отвязываем
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	/*glm::mat4 model;
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));*/

	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective<float>(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
	


	// Using key callbacks
	void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
	glfwSetKeyCallback(window, KeyCallback);

	// Game Loop
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClearColor(0.4f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader._program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader._program, "ourTexture2"), 1);
		ourShader.Use();
		//GLfloat timeValue = glfwGetTime();
		//GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		//GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// Transformation
		/*glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(sin((GLfloat)glfwGetTime() * 2.0f) / 2, 0.0f, 0.0f));
		trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));*/
		glm::mat4 model;
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(-0.5f, 0.2f, 1.0f));

		GLuint modelLoc = glGetUniformLocation(ourShader._program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLuint viewLoc = glGetUniformLocation(ourShader._program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		GLuint projectionLoc = glGetUniformLocation(ourShader._program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}




}