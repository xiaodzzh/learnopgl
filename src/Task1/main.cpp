#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//���ڴ�С�ı�ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//�������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}

#if 0
int main(int argc, char* argv[])
{

	if (!glfwInit())   //��ʼ��OpenGl
		return 1;

	//����OpenGl����

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL2 example", NULL, NULL);
	if (window == NULL)
		return 1;

	//����OpenGlɽ����
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

						 // ����ImGui������.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;


	//������ɫ���
	ImGui::StyleColorsDark();


	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		// Start the Dear ImGui frame ����IMgui Frame���.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		{
			//��ʼ����ImGui

			ImGui::Begin("IBinary Windows");                          // Create a window called "Hello, world!" and append into it.
			ImGui::Text("IBinary Blog");
			//ImGui::SameLine();
			ImGui::Indent(); //����һ���Ʊ����ʼ����Button
			ImGui::Button("2222", ImVec2(100, 50));

			ImGui::End();
		}

		// 3. Show another simple window.

		// Rendering
		ImGui::Render();


		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //�����ڻ�����Open֮����Ż���Imgui
																//glUseProgram(last_program);

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

#elif 1

int main(int argc, char* argv[])
{
	//��ʼ��
	if (glfwInit() == -1)
	{
		return -1;
	}
	//����������
	GLFWwindow* window = glfwCreateWindow(1200, 800, "learnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed to create Window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	//��ϵ������
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

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//����GLAD����OpenGLָ��
	gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

	//�ӿ�����
	glViewport(0, 0, 1200, 800);

	//���ô��ڴ�С�ı�ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Render Loop
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

		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //�����ڻ�����Open֮����Ż���Imgui
																//glUseProgram(last_program);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

#elif 2



#endif

