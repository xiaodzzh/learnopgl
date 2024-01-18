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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



#if 1

int main(int argc, char*argv[])
{

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(1600, 800, "learnOpenGL", NULL, NULL);


	glfwSwapInterval(1); // ���ô�ֱͬ������ֹ֡�ʹ���
	glfwMakeContextCurrent(window);

	// ����ImGui������.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//������ɫ���
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

	//�����ζ�������
	float vertices[] = {
		-0.1f, -0.1f, 0.0f, clear_color.x, 0.0f, 0.0f, 1.0f,
		0.1f, -0.1f, 0.0f, 0.0f, clear_color.y, 0.0f, 1.0f,
		0.0f,  0.1f, 0.0f, 0.0f, 0.0f, clear_color.z, 1.0f,
	};
	//���ζ�������
	float vertices1[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		0.5f,  0.9f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
		0.5f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
		-0.5f, -0.9f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
		-0.5f,  0.9f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};
	unsigned int indices[] = { 0,1,3,1,2,3 };
#if 0
	//----------������ɫ��--------------
	//������ɫ��
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
	//�ж���ɫ���Ƿ����ɹ�
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

	//Ƭ����ɫ��
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
	//�ж�Ƭ����ɫ���Ƿ����ɹ�
	{
		int success;
		char infoLog[512];
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
			std::cout << "Ƭ����ɫ������ʧ��:" << infoLog << std::endl;
			return 0;
		}
	}

	//����ɫ���������ӵ���ɫ������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	//�ж��Ƿ����ӳɹ�
	{
		int success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, NULL, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
			std::cout << "������ɫ���������:" << infoLog << std::endl;
		}
	}
	//������ɫ������

	////ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
#endif
	stbi_set_flip_vertically_on_load(true);

	//VAO����������󴴽�
	unsigned int VAO[2];
	glGenVertexArrays(2, VAO);

	//VBO���㻺����󴴽�
	unsigned int VBO[2];
	glGenBuffers(2, VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//��������
	unsigned int textures[2];
	glGenTextures(2, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���ز���������
	int width, height, nrChannels;
	unsigned char *data = stbi_load("./39.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���ز���������
	int width1, height1, nrChannels1;
	unsigned char *data1 = stbi_load("./39_1.jpg", &width1, &height1, &nrChannels1, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data1);

	glBindVertexArray(VAO[0]);
	//�������Ͱ�
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	//���������ݰ󶨵��������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//�󶨾��ζ�������
	glBindVertexArray(VAO[1]);
	//�������Ͱ�
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//
	//glUseProgram(shaderProgram);

	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glUseProgram(shaderProgram);
	float seed[2] = { 0, 0 };
	//glViewport(0, 0, 800, 400);
	while (!glfwWindowShouldClose(window))
	{
		// Start the Dear ImGui frame ����IMgui Frame���.
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

#elif 0
		//ʹ����ɫ����
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

		////���������ݰ󶨵��������
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(VAO[0]);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);
#endif

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
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		
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
		rtshader->setInt("texture1", index1, 1);
		rtshader->setInt("texture2", index2, 1);

		

		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
		//glfwSwapBuffers(window);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //�����ڻ�����Open֮����Ż���Imgui
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
