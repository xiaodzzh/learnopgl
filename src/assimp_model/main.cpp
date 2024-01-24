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
#include "CModel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define  SCREEN_WIDTH 2000
#define  SCREEN_HEIGHT 1000

static float fov = 45.0f;

static glm::vec3 cammove = glm::vec3(0.0f, 0.0f, -1.0f);

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightPos(0.0f, 0.0f, 14.0f);

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

		float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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
	ImVec4 clear_color = ImVec4(0.3f, 0.5f, 0.7f, 1.00f);
	gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

#if 0
	
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

	glEnable(GL_DEPTH_TEST);

	//设置回调事件
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	ImVec4 pLightcolor = { 1.0f,1.0f,1.0f, 1.0f };
	ImVec4 pObjectcolor = { 1.0f, 1.0f, 1.0f, 1.0f };


	CShader ourShader("./modelvert.vs", "./modelfrag.fs");

	// load models
	// -----------
	CModel ourModel("E:/learnopgl/src/assimp_model/resources/objects/backpack/backpack.obj");

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

			ImGui::SliderFloat("scale", &f, 0.0f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
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
			}
			else
			{
				ImGui::Text(("not use texture"));
			}
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.


		// Rendering
		ImGui::Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.Position = lightPos;
		//glClearColor(0.2f, 0.1f, 0.3f, 1.00f);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		std::cout << sin(glfwGetTime()) << std::endl;
		model = glm::rotate(model, (float)(glfwGetTime()), glm::vec3(0.0f,1.0f,0.0f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

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
