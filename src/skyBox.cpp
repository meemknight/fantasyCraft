#include "skyBox.h"
#include "stb_image/stb_image.h"

static float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};


void SkyBox::create()
{
	shader.loadFromFiles(RESOURCES_PATH "skyBox.vert", RESOURCES_PATH "skyBox.frag");

	u_cubeMap = glGetUniformLocation(shader.getId(), "u_cubeMap");
	u_viewProjection = glGetUniformLocation(shader.getId(), "u_viewProjection");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &cubeBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(0);
}

void SkyBox::clear()
{

	
}

void SkyBox::loadTextures(const char* textures[])
{
	if(cubemapTexture)
	{
		clearTexture();
	}

	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	int width, height, nrChannels;
	unsigned char *data;
	for (unsigned int i = 0; i < 6; i++)
	{
		stbi_set_flip_vertically_on_load(false);

		data = stbi_load(textures[i], &width, &height, &nrChannels, 0);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);

		stbi_image_free(data);

	}


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	
}

void SkyBox::render(const glm::mat4 &viewProjection)
{

	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(vao);
	shader.bind();
	
	glUniform1i(u_cubeMap, 0);
	glUniformMatrix4fv(u_viewProjection, 1, GL_FALSE, &viewProjection[0][0]);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glActiveTexture(GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

}

void SkyBox::clearTexture()
{
	glDeleteTextures(1, &cubemapTexture);
	cubemapTexture = 0;

}
