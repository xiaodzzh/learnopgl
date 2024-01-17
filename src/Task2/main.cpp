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
#if 1

int main(int argc, char*argv[])
{

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(1600, 800, "learnOpenGL", NULL, NULL);


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
	float vertices1[] = {
		0.2f, 0.3f, 0.0f,   // 右上角
		0.2f, 0.2f, 0.0f,  // 右下角
		-0.2f, 0.2f, 0.0f, // 左下角
		-0.2f, 0.3f, 0.0f   // 左上角
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
	//VAO顶点数组对象创建
	unsigned int VAO[2];
	glGenVertexArrays(2, VAO);

	//VBO顶点缓冲对象创建
	unsigned int VBO[2];
	glGenBuffers(2, VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO[0]);
	//缓冲类型绑定
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	//将顶点数据绑定到缓冲对象
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(VAO[1]);
	//缓冲类型绑定
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//
	//glUseProgram(shaderProgram);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glUseProgram(shaderProgram);

	//glViewport(0, 0, 800, 400);
	while (!glfwWindowShouldClose(window))
	{
		// Start the Dear ImGui frame 启动IMgui Frame框架.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
																	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
																	//ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.


		// Rendering
		ImGui::Render();
		glfwSwapBuffers(window);
		glfwPollEvents();

		glClearColor(0.2f, 0.1f, 0.3f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

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

#elif 1
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
#endif
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

		////将顶点数据绑定到缓冲对象
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(VAO[0]);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

#if 1
		CShader* rtshader = new CShader("./rtvshader.vs", "./rtfshader.fs");
		rtshader->use();
		float float4[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		rtshader->setFloat("clear_color", float4, 4);

		float rttimeValue1 = glfwGetTime();
		float rtofs = (sin(rttimeValue1));
		float rtcolor2[2] = { rtofs, cos(rttimeValue1) };
		rtshader->setFloat("offset", rtcolor2, 2);
#endif
		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
