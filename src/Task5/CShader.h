#ifndef CSHADER_H
#define CSHADER_H
#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/gtc/type_ptr.hpp"
class CShader
{
public:
	// 构造器读取并构建着色器
	CShader(const char* vertexPath, const char* fragmentPath);
	~CShader();

	// 使用/激活程序
	void use();
	// uniform工具函数

	//仅限一维数组
	void setBool(const std::string &name, bool value[], int size) const;
	void setInt(const std::string &name, int value[], int size) const;
	void setFloat(const std::string &name, float value[], int size) const;

	void setMatrix(const std::string &name, glm::mat4 trans) const;
private:
	unsigned int ID;
};

#endif CSHADER_H
