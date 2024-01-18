#ifndef CSHADER_H
#define CSHADER_H
#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/gtc/type_ptr.hpp"
class CShader
{
public:
	// ��������ȡ��������ɫ��
	CShader(const char* vertexPath, const char* fragmentPath);
	~CShader();

	// ʹ��/�������
	void use();
	// uniform���ߺ���

	//����һά����
	void setBool(const std::string &name, bool value[], int size) const;
	void setInt(const std::string &name, int value[], int size) const;
	void setFloat(const std::string &name, float value[], int size) const;

	void setMatrix(const std::string &name, glm::mat4 trans) const;
private:
	unsigned int ID;
};

#endif CSHADER_H
