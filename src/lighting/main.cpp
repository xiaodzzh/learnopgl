#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "CShader.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define  SCREEN_WIDTH 2000
#define  SCREEN_HEIGHT 1000

bool boolx = false;
bool booly = false;
static float lastx;
static float lasty;

bool firstMouse = true;

static float pitch = 0;
static float yaw = -90.0f;
static float fov = 45.0f;

static glm::vec3 cammove = glm::vec3(0.0f, 0.0f, -1.0f);

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 6.0f));

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

//定义鼠标移动回调事件
void mouse_callback(GLFWwindow* window, double x, double y)
{
	if (firstMouse)
	{
		lastx = x;
		lasty = y;
		firstMouse = false;
	}

	float xoffset, yoffset;
	float sensitivity = 0.1f;
	xoffset = static_cast<float>(x) - lastx;
	//鼠标相对于窗口左上角的位置，所以需要将y - lasty 取反
	yoffset = lasty - static_cast<float>(y);
	//std::cout << yoffset << std::endl;
	lastx = x;
	lasty = y;

	pitch += yoffset*sensitivity;

	yaw += xoffset*sensitivity;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	cammove[0] = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	cammove[1] = sin(glm::radians(pitch));
	cammove[2] = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	//cammove = glm::normalize(cammove);
}

#if 1

int main(int argc, char*argv[])
{

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "learnOpenGL", NULL, NULL);


	glfwSwapInterval(1); // 设置垂直同步，防止帧率过高
	glfwMakeContextCurrent(window);

	// 设置ImGui上下文.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//设置颜色风格
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	ImVec4 r_color;
	ImVec4 g_color;
	ImVec4 b_color;
	ImVec4 our_color;
	gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

	//三角形顶点坐标
	float vertices[] = {
		-0.1f, -0.1f, 0.0f, clear_color.x, 0.0f, 0.0f, 1.0f,
		0.1f, -0.1f, 0.0f, 0.0f, clear_color.y, 0.0f, 1.0f,
		0.0f,  0.1f, 0.0f, 0.0f, 0.0f, clear_color.z, 1.0f,
	};
	//矩形顶点坐标
	float vertices1[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	float vertices2[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = { 0,1,3,1,2,3 };
#if 0
	//----------编译着色器--------------
	//顶点着色器
	const char* verticeShaderSoure = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec4 my_color;\n"
		"uniform vec4 clear_color;\n"
		"uniform vec2 offset;\n"
		"out vec4 our_color;\n;"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x + offset.x, aPos.y - offset.y, aPos.z, 1);\n"
		"our_color = my_color;\n"
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &verticeShaderSoure, NULL);
	glCompileShader(vertexShader);
	//判断着色器是否编译成功
	{
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}

	//片段着色器
	const char* fragmentShaderSoure = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec4 our_color;\n"
		"void main()\n"
		"{\n"
		"   FragColor = our_color;\n"
		"}\n\0";
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSoure, NULL);
	glCompileShader(fragShader);
	//判断片段着色器是否编译成功
	{
		int success;
		char infoLog[512];
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
			std::cout << "片段着色器编译失败:" << infoLog << std::endl;
			return 0;
		}
	}

	//将着色器对象链接到着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	//判断是否链接成功
	{
		int success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, NULL, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
			std::cout << "链接着色器程序错误:" << infoLog << std::endl;
		}
	}
	//激活着色器程序

	////删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
#endif
	stbi_set_flip_vertically_on_load(true);

	//VAO顶点数组对象创建
	unsigned int VAO[4];
	glGenVertexArrays(4, VAO);

	//VBO顶点缓冲对象创建
	unsigned int VBO[3];
	glGenBuffers(3, VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//创建纹理
	unsigned int textures[4];
	textures[0] = loadTexture("./39.jpg");
	textures[1] = loadTexture("./39_2.jpg");
	textures[2] = loadTexture("./fs4.png");
	textures[3] = loadTexture("./39_1.jpg");

	glBindVertexArray(VAO[0]);
	//缓冲类型绑定
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	//将顶点数据绑定到缓冲对象
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

#if 1 //绑定光源
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
#endif

	//绑定矩形顶点属性
	glBindVertexArray(VAO[1]);
	//缓冲类型绑定
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//
	//glUseProgram(shaderProgram);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	//glUseProgram(shaderProgram);
	float seed[2] = { 0, 0 };
	//glViewport(0, 0, 800, 400);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//设置回调事件
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	CShader* rtshader = new CShader("./rtvshader.vs", "./rtfshader.fs");
	CShader* objectshader = new CShader("./objectvert.vs", "./objectfrag.fs");
	CShader* lightshader = new CShader("./lightvert.vs", "./lightfrag.fs");

	ImVec4 pLightcolor = { 1.0f,1.0f,1.0f, 1.0f };
	ImVec4 pObjectcolor = { 1.0f, 1.0f, 1.0f, 1.0f };



	bool useTexture[1] = { true };

	while (!glfwWindowShouldClose(window))
	{
		// Start the Dear ImGui frame 启动IMgui Frame框架.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		static float f = 0.5f;
		{

			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
																	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
																	//ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("scale", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			ImGui::Text("light");
			ImGui::ColorEdit3("", (float*)&pLightcolor);

			ImGui::Text("object");
			ImGui::ColorEdit3("1", (float*)&pObjectcolor);

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			if (counter % 2 != 0)
			{
				ImGui::Text("use texture");
				useTexture[0] = true;
			}
			else
			{
				ImGui::Text(("not use texture"));
				useTexture[0] = false;
			}
			objectshader->setBool("useTexture", useTexture, 1);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.


		// Rendering
		ImGui::Render();
		glfwSwapBuffers(window);
		glfwPollEvents();

		glClearColor(0.2f, 0.1f, 0.3f, 1.00f);
		//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 0
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "clear_color");
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		vertices[3] = clear_color.x * clear_color.w;
		vertices[11] = clear_color.y * clear_color.w;
		vertices[19] = clear_color.z * clear_color.w;
#elif 0
		float timeValue = glfwGetTime();
		float greenValue = std::abs((sin(timeValue)));
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "clear_color");
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, greenValue, greenValue, greenValue, 1.0f);

		float timeValue1 = glfwGetTime();
		float ofs = (sin(timeValue1));
		int vertexColorLocation1 = glGetUniformLocation(shaderProgram, "offset");
		glUseProgram(shaderProgram);
		glUniform2f(vertexColorLocation1, ofs, cos(timeValue1));

		vertices[3] = (1 - greenValue);
		vertices[11] = greenValue;
		vertices[19] = std::abs(std::cos(timeValue));

#elif 0
		//使用着色器类
		CShader* shader = new CShader("./vshader.vs", "./fshader.fs");
		shader->use();
		float timeValue = glfwGetTime();
		float greenValue = std::abs((sin(timeValue)));
		float color4[4] = { greenValue, greenValue, greenValue, 1.0f };
		shader->setFloat("clear_color", color4, 4);

		float timeValue1 = glfwGetTime();
		float ofs = (sin(timeValue1));
		float color2[2] = { ofs, cos(timeValue1) };
		shader->setFloat("offset", color2, 2);

		vertices[3] = (1 - greenValue);
		vertices[11] = greenValue;
		vertices[19] = std::abs(std::cos(timeValue));
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

		////将顶点数据绑定到缓冲对象
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(VAO[0]);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);
#endif

#if 1 //绘制光源
		{
			lightshader->use();
			//设置model变换矩阵
			glm::mat4 model = glm::mat4(1.0f);
			lightPos.x = !boolx ? lightPos.x + 0.02f : lightPos.x - 0.02f;
			lightPos.y = !booly ? lightPos.y + 0.01f : lightPos.y - 0.01f;
			if (lightPos.x > 3)
			{
				boolx = true;
			}
			if (lightPos.x < -3)
			{
				boolx = false;
			}
			if (lightPos.y > 2)
			{
				booly = true;
			}
			if (lightPos.y < -2)
			{
				booly = false;
			}
			model = glm::translate(model, glm::vec3(lightPos));
			
			model = glm::scale(model, glm::vec3(0.5f));
			lightshader->setMatrix("model", model);

			//设置view变换矩阵
			glm::mat4 view = camera.GetViewMatrix();
			//view = glm::translate(view, glm::vec3(0, 0, -3.0f));
			lightshader->setMatrix("view", view);

			//设置投影变换
			glm::mat4 projection = glm::mat4(1.0f);
			projection = glm::perspective(glm::radians(camera.Zoom), float(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.0f);
			lightshader->setMatrix("projection", projection);

			float pOutLightcolor[3] = { pLightcolor.x, pLightcolor.y, pLightcolor.z };
			lightshader->setFloat("outLightcolor", pOutLightcolor, 3);

			glBindVertexArray(VAO[3]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
#endif

#if 0
		
		rtshader->use();
		float float4[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		rtshader->setFloat("clear_color", float4, 4);

		float rttimeValue1 = glfwGetTime();
		float rtofs = (sin(rttimeValue1));
		float rtcolor2[2] = { rtofs, cos(rttimeValue1) };
		rtshader->setFloat("offset", rtcolor2, 2);
#endif
#if 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
#endif
		

#if 0
		seed[0] = abs(sin(rttimeValue1 / 6));
#elif 1
		if (glfwGetKey(window, GLFW_KEY_UP))
		{
			seed[0] = seed[0] > 1 ? 1 : seed[0] + 0.01;
			rtshader->setFloat("mixseed", seed, 2);
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
			seed[0] = seed[0] < 0 ? 0 : seed[0] - 0.01;
			rtshader->setFloat("mixseed", seed, 2);
		}
#endif


		rtshader->setFloat("mixseed", seed, 2);
		int index1[1] = { 0 };
		int index2[1] = { 1 };
		//rtshader->setInt("texture1", index1, 1);
		//rtshader->setInt("texture2", index2, 1);

		float timeValue = glfwGetTime();
		float trslWidth;
#if 1
		trslWidth = (sin(timeValue));
#elif 0
		trslWidth = 0.1f;
#endif


		//f = abs(trslWidth);

		//设置model变换矩阵
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(trslWidth, -trslWidth, 0));
		model = glm::rotate(model, float(trslWidth * 3.14), glm::vec3(trslWidth, -trslWidth, 1));
		model = glm::scale(model, glm::vec3(f, f, 1));
		rtshader->setMatrix("model", model);

		//设置view变换矩阵
		glm::mat4 view = camera.GetViewMatrix();
		//view = glm::translate(view, glm::vec3(0, 0, -3.0f));
		rtshader->setMatrix("view", view);

		//设置投影变换
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), float(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.0f);
		rtshader->setMatrix("projection", projection);

		//glBindVertexArray(VAO[1]);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//--------------------------------------------绘制立方体---------------------------------------------
#if 1
		objectshader->use();

#endif
#if 1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textures[3]);
#endif
		

#if 0
		seed[0] = abs(sin(rttimeValue1 / 6));
#elif 1
		if (glfwGetKey(window, GLFW_KEY_UP))
		{
			seed[0] = seed[0] > 1 ? 1 : seed[0] + 0.01;
			objectshader->setFloat("mixseed", seed, 2);
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
			seed[0] = seed[0] < 0 ? 0 : seed[0] - 0.01;
			objectshader->setFloat("mixseed", seed, 2);
		}
#endif


		objectshader->setFloat("mixseed", seed, 2);
		int index1t[1] = { 0 };
		int index2t[1] = { 1 };
		int index3t[1] = { 2 };
		int index4t[1] = { 3 };
		objectshader->setInt("texture1", index1t, 1);
		objectshader->setInt("texture2", index2t, 1);

		float timeValuet = glfwGetTime();
		float trslWidtht;
		objectshader->setInt("material.diffuse", index1t, 1);
		objectshader->setInt("material.specular", index2t, 1);
		objectshader->setInt("material.emsion", index3t, 1);
		objectshader->setInt("material.diftexture", index4t, 1);
#if 1
		trslWidtht = (sin(timeValuet));
#elif 0
		trslWidtht = 0.1f;
#endif
		objectshader->setFloat("light.constant", 1.0f);
		objectshader->setFloat("light.linear", 0.09f);
		objectshader->setFloat("light.quadratic", 0.032f);

		objectshader->setVec3("light.position", camera.Position);
		objectshader->setVec3("light.direction", camera.Front);
		objectshader->setFloat("light.cutOff", glm::cos(glm::radians(25.5f)));
		objectshader->setFloat("light.outterOff", glm::cos(glm::radians(30.f)));
		//f = abs(trslWidth);

		//设置model变换矩阵
		glm::mat4 modelt = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(trslWidth, -trslWidth, 0));
		//modelt = glm::rotate(modelt, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		modelt = glm::scale(modelt, glm::vec3(f, f, f));
		objectshader->setMatrix("model", modelt);

		glm::mat4 viewt = glm::mat4(1.0f);
#if 0
		//简单的设置一个view变换矩阵
		viewt = glm::translate(viewt, glm::vec3(0, 0, -3.0f));
#elif 0
		//通过设置lookat矩阵来实现view矩阵
		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		viewt = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
#elif 1
		//不同计算机可能每次绘制所需的时间不同，如果我们让计算机每次渲染移动固定的距离为固定值，则假如1s内，
		//渲染快的计算机移动的更快，这会导致不同计算机的效果不一样，因此我们要保证相同时间内都移动同样的距离
		static float movespeed = 0.0f;
		static float lastFrame = 0.0f;
		static glm::vec3 campos = glm::vec3(0.0f, 0.0f, 10.0f);
		glm::vec3 tt = campos + cammove;
		viewt = glm::lookAt(campos, tt, glm::vec3(0.0f, 1.0f, 0.0f));
		viewt = camera.GetViewMatrix();
		float currentFrame = glfwGetTime();
		movespeed = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//设置按键事件来控制摄像机视角
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			cammove[1] += movespeed;
			campos[1] += movespeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_W))
		{
			cammove[1] -= movespeed;
			campos[1] -= movespeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_A))
		{
			cammove[0] += movespeed;
			campos[0] += movespeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_D))
		{
			cammove[0] -= movespeed;
			campos[0] -= movespeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_Q))
		{
			cammove[2] -= movespeed;
			campos[2] -= movespeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_E))
		{
			cammove[2] += movespeed;
			campos[2] += movespeed;
		}
#endif
		objectshader->setMatrix("view", viewt);

		//设置投影变换
		glm::mat4 projectiont = glm::mat4(1.0f);
#if 0
		projectiont = glm::ortho(0.0f, 400.0f, 0.0f, 200.0f, 0.1f, 10.0f);
#elif 1
		projectiont = glm::perspective(glm::radians(fov), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 100.0f);
#endif
		
		objectshader->setMatrix("projection", projectiont);


		//glBindVertexArray(VAO[2]);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		for (int i = 0; i < 10; i++)
		{
			glBindVertexArray(VAO[2]);
			glm::mat4 modelt1 = glm::mat4(1.0f);
			float angle = i;
			modelt1 = glm::translate(modelt1, cubePositions[i]/*glm::vec3(sin(glfwGetTime()) + cubePositions[i][0], -sin(glfwGetTime()) + cubePositions[i][1], 0.0f + cubePositions[i][2])*/);
			//modelt1 = glm::rotate(modelt1, (float)(sin(glfwGetTime()) * 0.20)/*0.0f*/, glm::vec3(float(angle) / 10.0f, 1.0f -float(angle) / 10.0f, 1.0f));
			//modelt1 = glm::scale(modelt1, glm::vec3(f, f, f));
			objectshader->setMatrix("model", modelt1);

			glm::vec3 diffuseColor = glm::vec3(pLightcolor.x, pLightcolor.y, pLightcolor.z)  * glm::vec3(0.5f); // 降低影响
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响
			float pdiffuse[3] = { diffuseColor.x, diffuseColor.y, diffuseColor.z };
			float pambient[3] = { ambientColor.x, ambientColor.y, ambientColor.z };
			float pspe[3] = { 1.0,1.0,1.0 };
			objectshader->setFloat("light.ambient", pambient, 3);
			objectshader->setFloat("light.diffuse", pdiffuse, 3);
			objectshader->setFloat("light.specular", pspe, 3);

			// material properties
			glm::vec3 objectdiffuseColor = glm::vec3(pObjectcolor.x, pObjectcolor.y, pObjectcolor.z); // 降低影响
			float pobjdiffuse[3] = { objectdiffuseColor.x, objectdiffuseColor.y, objectdiffuseColor.z };
			float pobjambient[3] = { objectdiffuseColor.x * 0.1, objectdiffuseColor.y * 0.1, objectdiffuseColor.z * 0.1 };
			float pobjspe[3] = { 0.5f,0.5f,0.5f };
			float pobjshin[1] = { 32.0f };
			//objectshader->setFloat("material.ambient", pobjambient, 3);
			//objectshader->setFloat("material.diffuse", pobjdiffuse, 3);
			//objectshader->setFloat("material.specular", pobjspe, 3); // specular lighting doesn't have full effect on this object's material
			objectshader->setFloat("material.shininess", pobjshin, 1);


			/*float pTlight[3] = { pLightcolor.x, pLightcolor.y, pLightcolor.z };
			float pTobject[3] = { pObjectcolor.x, pObjectcolor.y, pObjectcolor.z };
			objectshader->setFloat("light_color", pTlight, 3);
			objectshader->setFloat("object_color", pTobject, 3);*/
			float pLightPos[3] = { lightPos.x, lightPos.y ,lightPos.z };
			objectshader->setFloat("light.position", pLightPos, 3);
			float pViewpos[3] = { camera.Position.x, camera.Position.y ,camera.Position.z };
			objectshader->setFloat("viewPos", pViewpos, 3);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		



		//glBindVertexArray(0);
		//glfwSwapBuffers(window);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //必须在绘制完Open之后接着绘制Imgui

	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO[0]);
	glDeleteBuffers(1, &VBO[0]);
	glDeleteBuffers(1, &VBO[1]);

	glfwTerminate();
	return 0;
}

#endif
