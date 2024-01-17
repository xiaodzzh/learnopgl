# OpenGL学习笔记

#### 1.你好，窗口

**opengl渲染流程：**

​	*初始化窗口->联系上下文->glad控制opengl指针->（渲染中）事件处理->渲染->置换前后缓冲区->渲染结束*

**疑问：**

​	*glviewport()是否在渲染具体画面上时才能生效?*

​	没错是的。

**代码：**

```
int main(int argc, char* argv[])
{
	//初始化
	if (glfwInit() == -1)
	{
		return -1;
	}
	//创建主窗口
	GLFWwindow* window = glfwCreateWindow(1200, 800, "learnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed to create Window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	//联系上下文
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

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//设置GLAD管理OpenGL指针
	gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

	//视口设置
	glViewport(0, 0, 100, 50);

	//设置窗口大小改变回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Render Loop
	while (!glfwWindowShouldClose(window))
	{

		// Start the Dear ImGui frame 启动IMgui Frame框架.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
```

​	
​			

```
			{
   				static float f = 0.0f;
   				static int counter = 0;
   	
```



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
   	
   			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //必须在绘制完Open之后接着绘制Imgui
   																	//glUseProgram(last_program);
   		}
   	
   		// Cleanup
   		ImGui_ImplOpenGL3_Shutdown();
   		ImGui_ImplGlfw_Shutdown();
   		ImGui::DestroyContext();
   	
   		glfwTerminate();
   		return 0;
   	}

#### 2.你好，三角形

顶点数组对象VAO，顶点缓冲对象VBO，元素缓冲对象EBO

渲染一个三角形流程：定义VAO,VBO,EBO->绑定VAO,VBO,EBO->解绑->再绑定不同的VAO,VBO,EBO->渲染

###### （1）定义VAO

```
unsigned int VAOS[2];
glGenVertexArrays(2, VAOS);
```

###### （2）定义VBO,EBO

```
unsigned int VBOS[2];
glGenBuffers(2, VBOS);
unsigned int EBO;
glGenBuffers(1, &EBO);
```

###### （3）绑定

```
glBindVertexArray(VAO[0]);
//缓冲类型绑定
glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

//将顶点数据绑定到缓冲对象
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//设置顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
glEnableVertexAttribArray(0);
```

###### （4）索引绑定

```
glBindVertexArray(VAO[1]);
//缓冲类型绑定
glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
glEnableVertexAttribArray(0);
```

###### （5）解绑

```
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);
```

###### （6）渲染

```
glBindVertexArray(VAO[0]);
glDrawArrays(GL_TRIANGLES, 0, 3);
//glBindVertexArray(0);
glBindVertexArray(VAO[1]);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//glBindVertexArray(0);
```

#### 3.着色器

###### （1）shader类型

​	顶点着色器，片段着色器(分别简写为vshader,fshader)

​	vshader负责将顶点属性作为输入进行变换然后传递给GPU，fshader负责控制颜色。

###### （2）shader常用语法

```
in vec3 my_color;#作为输入
out vec4 out_color;#作为输出
uniform vec3 uni_color;#类似于变量，可以通过程序改变
layout (location = 0) in vec3 aPos;#将顶点属性aPos放到位置0处
```

###### （3）一个简单的Shader

```
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 my_color;
uniform vec4 clear_color;
uniform vec2 offset;
out vec4 our_color;
void main()
{
    gl_Position = vec4(aPos.x + offset.x, aPos.y - offset.y, aPos.z, 1);
	our_color = my_color;
};
```

###### （4）如何使用Shader

​	编译着色器->创建着色器程序->链接->使用着色器程序->删除着色器

```
//编译着色器
unsigned int vshader;
vshader = glCreateShader(GL_VERTEX_SHADER);
glShaderSoure(vertex, 1, &soureCode, NULL);
glCompileShader(vshader);
//创建着色器程序
unsigned int ID = glCreateProgram();
//链接
glAttachShader(ID, vshader);
glAttachShader(ID, fshader);
//使用着色器程序
glUseProgram(ID);
//删除着色器,后续只会使用到着色器程序
glDeleteShader(vshader);
glDeleteShader(fshader);
```

###### （5）不同的物体使用不同的着色器绘制时

​	先使用着色器程序再绘制，否则绘制的是最近的那个着色器程序。

#### 4.纹理



