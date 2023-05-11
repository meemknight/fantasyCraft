#include "shader.h"
#include <fstream>
#include <iostream>
#include "tools.h"
#include "log.h"

void Shader::loadFromFiles(std::string vertexPath, std::string fragmentPath)
{
	std::string vertexSource = loadShaderSource(vertexPath);
	auto vertexShaderId = createShaderFromMemory(vertexSource.c_str(), GL_VERTEX_SHADER);

	std::string fragmentSource = loadShaderSource(fragmentPath);
	auto fragmentShaderId = createShaderFromMemory(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

	id = glCreateProgram();

	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);

	glLinkProgram(id);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	int info = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &info);

	if (info != GL_TRUE)
	{
		char *message = 0;
		int   l = 0;

		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);

		message = new char[l];

		glGetProgramInfoLog(id, l, &l, message);

		llog(ErrorLog(), message, "\n", "in:", vertexPath, "and:", fragmentPath);

		delete[] message;
	}

	glValidateProgram(id);

}

GLuint Shader::createShaderFromMemory(const char *data, GLenum type)
{
	GLuint shaderID = glCreateShader(type);

	glShaderSource(shaderID, 1, &data, 0);
	glCompileShader(shaderID);


	int result = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		char *message = 0;
		int   l = 0;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &l);

		if (l)
		{

			message = new char[l];

			glGetShaderInfoLog(shaderID, l, &l, message);

			message[l - 1] = 0;

			llog(ErrorLog(), message, "\n", "data:\n\n", data);

			delete[] message;

		}

	}

	return shaderID;
}

std::string Shader::loadShaderSource(std::string source)
{
	std::ifstream f(source);

	if (!f.is_open())
	{
		throw(std::string("err loading ") + source);
	}

	//most vexing parse here yay love cpp
	std::string ret{ std::istreambuf_iterator<char>(f), {} };

	return std::move(ret);
}

void DrawBlocksShader::load()
{
	loadFromFiles(RESOURCES_PATH "vertex.vert", RESOURCES_PATH "fragment.frag");

	permaAssertComment(id, "shader not found");

	u_playerPos = glGetUniformLocation(id, "u_playerPos");
	u_modelView = glGetUniformLocation(id, "u_modelView");
	u_projectionMatrix = glGetUniformLocation(id, "u_projectionMatrix");
	u_texture = glGetUniformLocation(id, "u_texture");
	u_pos = glGetUniformLocation(id, "u_pos");
	u_atlas = glGetUniformLocation(id, "u_atlas");
	u_ao = glGetUniformLocation(id, "u_ao");


	if (u_projectionMatrix == -1)
	{
		llog(ErrorLog(), "projMat error\n");
	}

}

void DrawBlocksShader::setProjectionMatrix(const glm::mat4 &mat)
{
	glUniformMatrix4fv(u_projectionMatrix, 1, GL_FALSE, &(mat[0][0]));

}

void DrawBlocksShader::setModelViewMatrix(const glm::mat4 &mat)
{
	glUniformMatrix4fv(u_modelView, 1, GL_FALSE, &(mat[0][0]));

}

void DrawBlocksShader::setPlayerPos(const glm::vec3 &pos)
{
	glUniform3f(u_playerPos, pos.x, pos.y, pos.z);
}

void DrawBlocksShader::setTexture(int index)
{
	glUniform1i(u_texture, index);
}

void DrawBlocksShader::setPosition(int x, int y, int z)
{
	glUniform3i(u_pos, x, y, z);
}

void DrawBlocksShader::setTextureAtlasCoords(int x, int y)
{
	glUniform2i(u_atlas, x, y);
}


void DrawBlocksShader::setAo(bool ao)
{
	glUniform1i(u_ao, ao);
}
