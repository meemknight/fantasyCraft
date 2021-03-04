#include <glad/glad.h>
#include <string>

class Shader
{
public:
	Shader() {};

	void loadFromFiles(std::string vertexPath, std::string fragmentPath);

	void clear()
	{
		glDeleteProgram(id);
	};
	
	void bind()
	{
		glUseProgram(id);
	}

private:

	GLuint id = 0;

	GLuint createShaderFromMemory(const char *data, GLenum type);
	std::string loadShaderSource(std::string source);

};