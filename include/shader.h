#pragma once
//todo switch

#include <glad/glad.h>
#include <string>
#include <glm/mat4x4.hpp>
#include <type_traits>

class Shader
{
public:
	Shader() {};

	void loadFromFiles(std::string vertexPath, std::string fragmentPath);

	void bind()
	{
		glUseProgram(id);
	}

	void clear()
	{
		glDeleteProgram(id);
	}

	GLuint getId() { return id; }

protected:

	GLuint id = 0;

	GLuint createShaderFromMemory(const char *data, GLenum type);
	std::string loadShaderSource(std::string source);

};

class DrawBlocksShader : public Shader
{
public:
	void load();

	void setProjectionMatrix(const glm::mat4 &mat);
	void setModelViewMatrix(const glm::mat4 &mat);
	void setPlayerPos(const glm::vec3 &pos);
	void setTexture(int index);
	void setPosition(int x, int y, int z);
	void setTextureAtlasCoords(int x, int y);
	void setAo(bool ao);

private:

	GLint u_playerPos;
	GLint u_modelView;
	GLint u_projectionMatrix;
	GLint u_texture;
	GLint u_pos;
	GLint u_atlas;
	GLint u_ao;

};