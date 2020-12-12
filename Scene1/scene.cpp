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

// --------------------------------------------------------------------------------------------------------------------------- //

bool keys[1024];
bool rightMouseFlag, firstMouseCallback = true;
GLfloat lastMouseX = 400, lastMouseY = 300, deltaTime = 0.0f, lastFrame = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseButtonCallback(GLFWwindow* window, int key, int action, int mode);
void Move();
void Zoom();


// --------------------------------------------------------------------------------------------------------------------------- //

int main() {

	int frameCount = 0;
	const float nearPlane = 1.0f, farPlane = 20.0f;
	double currSecond, prevSecond = glfwGetTime();
	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projectionLoc;
	glm::mat4 model, view, projection, tangent;
	glm::mat4 lightProjection, lightView, lightViewProjection;

	// Текстуры для Skybox
	vector<std::string> skyboxFaces = {
	"textures/right.jpg",
	"textures/left.jpg",
	"textures/top.jpg",
	"textures/bottom.jpg",
	"textures/front.jpg",
	"textures/back.jpg"
	};

	// Основной параллельный источник освещения
	//glm::vec3 lightPosition(-3.6f, 2.5f, -2.5f);
	glm::vec3 lightPosition(-3.6f, 2.5f, 2.5f);
	glm::vec3 lightAmbient(0.1f, 0.1f, 0.1f);
	glm::vec3 lightDiffuse(1.0f, 0.8f, 0.6f);
	glm::vec3 lightSpecular(1.0f, 0.9f, 0.8f);
	glm::vec3 lightDir = glm::normalize(-lightPosition);


	// Инициализация GLFW
	glfwInit();
	glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
	//glfwWindowHint ( GLFW_DEPTH_BITS, 24 );
	//glfwWindowHint ( GLFW_CLEINT_API, GLFW_OPENGL_API );
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


	// Окно
	int screenWidth, screenHeight;
	GLFWwindow* window = glfwCreateWindow(1200, 800, "Beautiful scene", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	// Success Check
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}


	// ВКЛЮЧЕНИЕ DEPTH TEST
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_GREATER);


	// Шейдеры
	Shader lightingShader("shaders/lighting.vs", "shaders/lighting.frag");
	Shader emissionShader("shaders/emission.vs", "shaders/emission.frag");
	Shader lightShadowShader("shaders/light_shadow.vs", "shaders/light_shadow.frag");
	Shader shadowShader("shaders/shadow.vs", "shaders/shadow.frag");
	Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.frag");
	Shader postShader("shaders/post.vs", "shaders/post.frag");
	Shader gaussShader("shaders/gauss.vs", "shaders/gauss.frag");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	// Загрузка всех моделей
	//Mesh mesh_bricks(PLANE, )
	Texture meshTextures[] = { Texture(DIFFUSE, "textures/bricks.jpg"), Texture(NORMAL, "textures/bricks_normal.jpg") };
	//Texture meshTextures[] = { Texture(NORMAL, "textures/bricks_normal.jpg"), Texture(DIFFUSE, "textures/bricks.jpg") };
	Mesh mesh_bricks(PLANE, 2, meshTextures);
	Model model_shrek("models/CHARACTER_Shrek.obj");
	Model model_ground("models/10450_Rectangular_Grass_Patch_v1_iterations-2.obj"); 
	model_ground.meshes[0].hasNormal = false;
	CubeMap skybox(skyboxFaces);

	// Инициализация кадрового буфера и текстуры для теней относительно света
	unsigned int lightShadowMapFB, lightShadowMap;
	const unsigned int shadowTextureWidth = 4096, shadowTextureHeight = 4096;
	glGenFramebuffers(1, &lightShadowMapFB);
	glGenTextures(1, &lightShadowMap);
	glBindTexture(GL_TEXTURE_2D, lightShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowTextureWidth, shadowTextureHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, lightShadowMapFB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, lightShadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Инициализация кадрового буфера для изображения перед пост обработкой
	unsigned int frameTextureFB, frameTexture;
	glGenFramebuffers(1, &frameTextureFB);
	glGenTextures(1, &frameTexture);
	glBindTexture(GL_TEXTURE_2D, frameTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, frameTextureFB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture, 0);

	//Инициализация рендер буфера для frameTextureFB
	/*unsigned int RB;
	glGenRenderbuffers(1, &RB);
	glBindRenderbuffer(GL_RENDERBUFFER, RB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RB);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "Error: renderbuffer is not complete!" << endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);*/

	// Инициализация кадрового буфера и текстуры для теней относительно камеры
	unsigned int shadowMapFB, shadowMap;
	glGenFramebuffers(1, &shadowMapFB);
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowMap, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int RB0;
	glGenRenderbuffers(1, &RB0);
	glBindRenderbuffer(GL_RENDERBUFFER, RB0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RB0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "Error: renderbuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Кадровый буфер для размытой карты теней
	unsigned int blurShadowMapFB, blurShadowMap;
	glGenFramebuffers(1, &blurShadowMapFB);
	glGenTextures(1, &blurShadowMap);
	glBindTexture(GL_TEXTURE_2D, blurShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, blurShadowMapFB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, blurShadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Создание прямоугольника, на который будет натягиваться итоговая текстура
	Mesh screenQuad(shadowMap);

	// Using key callbacks
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// --------------------------------------------------------------------------------------------------------------------------- //

	// Game Loop
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		Move();
		Zoom();

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		// Работа с источником света
		//lightSpecular.y = (sin((GLfloat)glfwGetTime()) + 1) * 0.5f;
		//lightSpecular.z = (sin((GLfloat)glfwGetTime()) + 1) * 0.5f;
		//lightPosition.z = 3.0f;
		//lightPosition.y = 4.0f;
		//lightPosition.x = (sin(0.6f * (GLfloat)glfwGetTime())) * 10.0f;
		glm::vec3 lightDir = glm::normalize(-lightPosition);


		// Первая отрисовка: Получение буфера глубины для теней
		lightShadowShader.Use();
		lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, nearPlane, farPlane);
		lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightViewProjection = lightProjection * lightView;
		lightShadowShader.SetMat4("lightViewProjection", lightViewProjection);
		glViewport(0, 0, shadowTextureWidth, shadowTextureHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, lightShadowMapFB);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		
		model = glm::mat4();
		lightShadowShader.SetMat4("model", model);
		model_shrek.Draw(lightShadowShader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (glm::radians)(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightShadowShader.SetMat4("model", model);
		model_shrek.Draw(lightShadowShader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.14f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		model = glm::rotate(model, (glm::radians)(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		lightShadowShader.SetMat4("model", model);
		model_ground.Draw(lightShadowShader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -0.5f));
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, (glm::radians)(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		lightShadowShader.SetMat4("model", model);
		mesh_bricks.Draw(lightShadowShader);


		// Возврат к настройкам окна
		glViewport(0, 0, screenWidth, screenHeight);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);







		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadowShader.Use();
		shadowShader.SetInt("lightShadowMap", 10);
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, lightShadowMap);

		view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		projection = glm::perspective<float>(glm::radians(camera.fov), (float)screenWidth / screenHeight, 0.1f, 20.0f);
		shadowShader.SetMat4("view", view);
		shadowShader.SetMat4("projection", projection);
		shadowShader.SetMat4("lightViewProjection", lightViewProjection);

		//glDepthFunc(GL_GREATER);

		model = glm::mat4();
		//model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, (glm::radians)(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		shadowShader.SetMat4("model", model);
		model_shrek.Draw(shadowShader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (glm::radians)(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shadowShader.SetMat4("model", model);
		model_shrek.Draw(shadowShader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.14f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		model = glm::rotate(model, (glm::radians)(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		shadowShader.SetMat4("model", model);
		model_ground.Draw(shadowShader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -0.5f));
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, (glm::radians)(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (glm::radians)(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shadowShader.SetMat4("model", model);
		mesh_bricks.Draw(shadowShader);






		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gaussShader.Use();
		gaussShader.SetInt("tex", 11);
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		

		glBindVertexArray(screenQuad.VAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)screenQuad.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0);

		//screenQuad.Draw(gaussShader);
		glEnable(GL_DEPTH_TEST);




		/*
		// Вторая отрисовка
		glBindFramebuffer(GL_FRAMEBUFFER, frameTextureFB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lightingShader.Use();
		lightingShader.SetVec3("light.ambient", lightAmbient);
		lightingShader.SetVec3("light.diffuse", lightDiffuse);
		lightingShader.SetVec3("light.specular", lightSpecular);
		lightingShader.SetVec3("lightDirection", lightDir);
		lightingShader.SetInt("shadowMap", 10);
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, lightShadowMap);
		                    
		view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		projection = glm::perspective<float>(glm::radians(camera.fov), (float)screenWidth / screenHeight, 0.1f, 20.0f);
		tangent = glm::mat4();
		tangent = glm::rotate(tangent, (glm::radians)(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		lightingShader.SetMat4("view", view);
		lightingShader.SetMat4("projection", projection);
		lightingShader.SetMat4("lightViewProjection", lightViewProjection);
		lightingShader.SetVec3("cameraPosition", camera.position);
		lightingShader.SetMat4("tangentMatrix", tangent);

		
		model = glm::mat4();
		//model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, (glm::radians)(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		lightingShader.SetMat4("model", model);
		model_shrek.Draw(lightingShader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (glm::radians)(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.SetMat4("model", model);
		model_shrek.Draw(lightingShader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.14f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		model = glm::rotate(model, (glm::radians)(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		lightingShader.SetMat4("model", model);
		model_ground.Draw(lightingShader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -0.5f));
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, (glm::radians)(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (glm::radians)(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		lightingShader.SetMat4("model", model);
		mesh_bricks.Draw(lightingShader);  */


		/*
		// Отрисовка Skybox
		skyboxShader.Use();
		glDepthFunc(GL_LEQUAL);
		view = glm::mat4(glm::mat3(view)); // Убирает из матрицы информацию о перемещении
		skyboxShader.SetMat4("view", view);
		skyboxShader.SetMat4("projection", projection);
		skybox.Draw(skyboxShader);
		glDepthFunc(GL_LESS);   */
		 

		/*
		// Отрисовка итогового изображения
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		postShader.Use();
		screenQuad.Draw(postShader);
		glEnable(GL_DEPTH_TEST);
		*/


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

// --------------------------------------------------------------------------------------------------------------------------- //

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