#include "CShader.h"

CShader::CShader(const char * vertexPath, const char * fragmentPath)
{
	// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ��֤ifstream��������׳��쳣��
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// ��ȡ�ļ��Ļ������ݵ���������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// �ر��ļ�������
		vShaderFile.close();
		fShaderFile.close();
		// ת����������string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. ������ɫ��
	unsigned int vertex, fragment;
	char infoLog[512];

	// ������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// ��ӡ�����������еĻ���
	{
		int success;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};
	}

	// Ƭ����ɫ��Ҳ����

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//�ж�Ƭ����ɫ���Ƿ����ɹ�
	{
		int success;
		char infoLog[512];
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "Ƭ����ɫ������ʧ��:" << infoLog << std::endl;
		}
	}

	// ��ɫ������
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	{
		int success;
		// ��ӡ���Ӵ�������еĻ���
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
	

	// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
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
