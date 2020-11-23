#define GLEW_STATIC
#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"


GLfloat triangle01[] = {
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  
};

GLfloat rectangle01[] = {
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    
};

GLfloat cube01[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -45.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);
glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

GLuint indices[] = { 
	0, 1, 3, 
	1, 2, 3
};

bool firstMouseCallback = true;

float ambientStrength = 0.1f;

// Delta time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Mouse Positions
GLfloat lastMouseX = 400, lastMouseY = 300;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// keys
bool keys[1024];
bool rightMouseFlag;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseButtonCallback(GLFWwindow* window, int key, int action, int mode);
void Move();
void Zoom();


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

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Window
	GLFWwindow* window = glfwCreateWindow(1200, 800, "Beautiful scene", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	Shader lightingShader("shaders/lighting.vs", "shaders/lighting.frag");
	Shader emissionShader("shaders/emission.vs", "shaders/emission.frag");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	// Vertex Buffer Object, Vertex Array Object, Element Buffer Object
	// Initialization
	GLuint VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);

	// -------------------------------------------------------------------------------------------------------------------- //
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube01), cube01, GL_STATIC_DRAW);
	// object 2
	// object 3
	// object ...




	// -------------------------------------------------------------------------------------------------------------------- //

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Texture 1
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int texWidth, texHeight;
	unsigned char* image = SOIL_load_image("textures/container2.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
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
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("textures/container2_specular.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
	for (int i = 0; i < 64; i++) std::cout << "2: " << (int)image[i] << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Отвязываем
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	lightingShader.Use();
	lightingShader.SetInt("material.diffuse", texture1);
	lightingShader.SetInt("material.specular", texture2);

	//GLint lightPositionLoc = glGetUniformLocation(lightingShader._program, "lightPosition");
	//glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);

	glm::mat4 view;
	glm::mat4 projection;

	// Using key callbacks
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// Game Loop
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		Move();
		Zoom();

		glClearColor(0.025f, 0.025f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		lightDiffuse.y = (sin((GLfloat)glfwGetTime()) + 1) * 0.25f;
		lightDiffuse.z = (sin((GLfloat)glfwGetTime()) + 1) * 0.25f;

		lightAmbient.y = (sin((GLfloat)glfwGetTime()) + 1) * 0.1f;
		lightAmbient.z = (sin((GLfloat)glfwGetTime()) + 1) * 0.1f;

		lightSpecular.y = (sin((GLfloat)glfwGetTime()) + 1) * 0.5f;
		lightSpecular.z = (sin((GLfloat)glfwGetTime()) + 1) * 0.5f;


		lightingShader.Use();

		lightingShader.SetVec3("material.ambient", 0.2f, 0.2f, 0.4f);
		lightingShader.SetInt("material.diffuse", texture1);
		lightingShader.SetInt("material.specular", texture2);
		lightingShader.SetFloat("material.shininess", 32.0f);

		lightingShader.SetVec3("light.ambient", lightAmbient);
		lightingShader.SetVec3("light.diffuse", lightDiffuse); 
		lightingShader.SetVec3("light.specular", lightSpecular);

		lightingShader.SetVec3("_lightPosition", lightPosition);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1); 
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		                                         
		/*glUniform1i(glGetUniformLocation(ourShader._program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader._program, "ourTexture2"), 1);
		ourShader.Use();*/
		//GLfloat timeValue = glfwGetTime();
		//GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		//GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// Transformations
		view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		projection = glm::perspective<float>(glm::radians(camera.fov), (float)width / height, 0.1f, 100.0f);

		GLuint modelLoc = glGetUniformLocation(lightingShader._program, "model");
		GLuint viewLoc = glGetUniformLocation(lightingShader._program, "view");
		GLuint projectionLoc = glGetUniformLocation(lightingShader._program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Object
		glBindVertexArray(VAO);
		glm::mat4 model;
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);



		emissionShader.Use();
		emissionShader.SetVec3("lightColor", lightDiffuse);
		modelLoc = glGetUniformLocation(emissionShader._program, "model");
		viewLoc = glGetUniformLocation(emissionShader._program, "view");
		projectionLoc = glGetUniformLocation(emissionShader._program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f));

		glBindVertexArray(lightVAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);



		glfwSwapBuffers(window);

		// Calculation Delta Time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

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

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void MouseButtonCallback(GLFWwindow* window, int key, int action, int mode) {
	if (key == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS)
			rightMouseFlag = true;
		else
			rightMouseFlag = false;
	}
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouseCallback) {
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouseCallback = false;
	}

	GLfloat xoffset = xpos - lastMouseX;
	GLfloat yoffset = ypos - lastMouseY;
	lastMouseX = xpos;
	lastMouseY = ypos;

	camera.Rotate(xoffset, yoffset);
}

void Move() {
	if (keys[GLFW_KEY_W])
		camera.Move(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.Move(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.Move(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.Move(RIGHT, deltaTime);
}

void Zoom() {
	camera.Zoom(rightMouseFlag);
}