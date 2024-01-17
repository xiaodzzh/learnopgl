#include "CShader.h"

CShader::CShader(const char * vertexPath, const char * fragmentPath)
{
	// 1. 从文件路径中获取顶点/片段着色器
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		// 转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. 编译着色器
	unsigned int vertex, fragment;
	char infoLog[512];

	// 顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// 打印编译错误（如果有的话）
	{
		int success;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};
	}

	// 片段着色器也类似

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//判断片段着色器是否编译成功
	{
		int success;
		char infoLog[512];
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "片段着色器编译失败:" << infoLog << std::endl;
		}
	}

	// 着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	{
		int success;
		// 打印连接错误（如果有的话）
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
	

	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void CShader::use()
{
	glUseProgram(ID);
}

void CShader::setBool(const std::string & name, bool value[], int size) const
{
	switch (size)
	{
	case 1:
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value[0]);
		break;
	case 2:
		glUniform2i(glGetUniformLocation(ID, name.c_str()), (int)value[0], (int)value[1]);
		break;
	case 3:
		glUniform3i(glGetUniformLocation(ID, name.c_str()), (int)value[0], (int)value[1], (int)value[2]);
		break;
	case 4:
		glUniform4i(glGetUniformLocation(ID, name.c_str()), (int)value[0], (int)value[1], (int)value[2], (int)value[3]);
		break;
	default:
		break;
	}
}

void CShader::setInt(const std::string & name, int value[], int size) const
{
	switch (size)
	{
	case 1:
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value[0]);
		break;
	case 2:
		glUniform2i(glGetUniformLocation(ID, name.c_str()), (int)value[0], (int)value[1]);
		break;
	case 3:
		glUniform3i(glGetUniformLocation(ID, name.c_str()), (int)value[0], (int)value[1], (int)value[2]);
		break;
	case 4:
		glUniform4i(glGetUniformLocation(ID, name.c_str()), (int)value[0], (int)value[1], (int)value[2], (int)value[3]);
		break;
	default:
		break;
	}
}

void CShader::setFloat(const std::string & name, float value[], int size) const
{
	switch (size)
	{
	case 1:
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value[0]);
		break;
	case 2:
		glUniform2f(glGetUniformLocation(ID, name.c_str()), value[0], value[1]);
		break;
	case 3:
		glUniform3f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2]);
		break;
	case 4:
		glUniform4f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2], value[3]);
		break;
	default:
		break;
	}
}
