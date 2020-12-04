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
#include "Model.h"
#include "CubeMap.h"


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


glm::vec3 lightPosition(-5.0f, 5.0f, -10.0f);
glm::vec3 lightAmbient(0.1f, 0.1f, 0.1f);
glm::vec3 lightDiffuse(1.0f, 1.0f, 1.0f);
glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

GLuint indices[] = { 
	0, 1, 3, 
	1, 2, 3
};

vector<std::string> skyboxFaces = {
	"textures/right.jpg",
	"textures/left.jpg",
	"textures/top.jpg",
	"textures/bottom.jpg",
	"textures/front.jpg",
	"textures/back.jpg"
};

bool firstMouseCallback = true;

float ambientStrength = 0.1f;

// Delta time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Mouse Positions
GLfloat lastMouseX = 400, lastMouseY = 300;

Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

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
	//glDepthFunc(GL_LESS); // Эта строчка работает по умолчанию

	// Shader
	Shader lightingShader("shaders/lighting.vs", "shaders/lighting.frag");
	Shader emissionShader("shaders/emission.vs", "shaders/emission.frag");
	Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.frag");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Loading model
	Model model_shrek("models/CHARACTER_Shrek.obj");
	Model model_ground("models/10450_Rectangular_Grass_Patch_v1_iterations-2.obj");

	// Creating skybox
	CubeMap skybox(skyboxFaces);


	//skyboxShader.Use();
	//skyboxShader.SetInt("skybox", 0);
	//cout << "skybox.texture = " << skybox.texture << std::endl;
	//lightingShader.SetInt("diffuseTexture1", skybox.texture);


	glm::mat4 model;
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



		//lightDiffuse.y = (sin((GLfloat)glfwGetTime()) + 1) * 0.25f;
		//lightDiffuse.z = (sin((GLfloat)glfwGetTime()) + 1) * 0.25f;

		//lightAmbient.y = (sin((GLfloat)glfwGetTime()) + 1) * 0.1f;
		//lightAmbient.z = (sin((GLfloat)glfwGetTime()) + 1) * 0.1f;

		//lightSpecular.y = (sin((GLfloat)glfwGetTime()) + 1) * 0.5f;
		//lightSpecular.z = (sin((GLfloat)glfwGetTime()) + 1) * 0.5f;


		lightingShader.Use();

		/*lightingShader.SetVec3("material.ambient", 0.2f, 0.2f, 0.4f);
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
		glBindTexture(GL_TEXTURE_2D, texture2);*/

		//lightingShader.SetInt("material.diffuse", texture1);
		//lightingShader.SetInt("material.specular", texture2);

		glm::vec3 diffuse = glm::vec3(0.6f, 0.3f, 0.3f);
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

		//lightingShader.SetVec3("material.diffuse", diffuse);
		//lightingShader.SetVec3("material.specular", specular);
		//lightingShader.SetFloat("material.shininess", 32.0f);

		lightingShader.SetVec3("light.ambient", lightAmbient);
		lightingShader.SetVec3("light.diffuse", lightDiffuse);
		lightingShader.SetVec3("light.specular", lightSpecular);

		lightingShader.SetVec3("_lightPosition", lightPosition);
		                                       

		// Transformations
		model = glm::mat4();
		view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		projection = glm::perspective<float>(glm::radians(camera.fov), (float)width / height, 0.1f, 100.0f);

		GLuint modelLoc = glGetUniformLocation(lightingShader._program, "model");
		GLuint viewLoc = glGetUniformLocation(lightingShader._program, "view");
		GLuint projectionLoc = glGetUniformLocation(lightingShader._program, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Object
		//glBindVertexArray(VAO);
		model_shrek.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, -0.14f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		model = glm::rotate(model, (glm::radians)(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		model_ground.Draw(lightingShader);
		//glBindVertexArray(0);


		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(view)); // Убирает из матрицы информацию о перемещении
		GLuint skyboxViewLoc = glGetUniformLocation(skyboxShader._program, "view");
		GLuint skyboxProjectionLoc = glGetUniformLocation(skyboxShader._program, "projection");
		glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(skyboxProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		skybox.Draw(skyboxShader);
		glDepthFunc(GL_LESS);


		/*emissionShader.Use();
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
		glBindVertexArray(0);*/



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

	glDeleteVertexArrays(1, &(skybox.VAO));
	glDeleteBuffers(1, &(skybox.VAO));

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
		lastMouseX = (GLfloat)xpos;
		lastMouseY = (GLfloat)ypos;
		firstMouseCallback = false;
	}

	GLfloat xoffset = (GLfloat)xpos - lastMouseX;
	GLfloat yoffset = (GLfloat)ypos - lastMouseY;
	lastMouseX = (GLfloat)xpos;
	lastMouseY = (GLfloat)ypos;

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