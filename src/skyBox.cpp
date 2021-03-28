#include "skyBox.h"
#include "stb_image/stb_image.h"
#include <iostream>

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


void SkyBox::loadTexturesFromCrossTexture(const char *texture)
{
	if (cubemapTexture)
	{
		clearTexture();
	}

	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	int width, height, nrChannels;
	unsigned char *data;


	stbi_set_flip_vertically_on_load(false);
	data = stbi_load(texture, &width, &height, &nrChannels, 3);

	//right
	//left
	//top
	//bottom
	//front
	//back

	auto getPixel = [&](int x, int y, unsigned char *data)
	{
		return (unsigned char*)(data + 3 * (x + y * width));
	};

	glm::ivec2 paddings[6];
	glm::ivec2 immageRatio = {};

	{
		immageRatio = { 4, 3 };
		glm::ivec2 paddingscopy[6] =
		{
			{ (width / 4) * 2, (height / 3) * 1, },
			{ (width / 4) * 0, (height / 3) * 1, },
			{ (width / 4) * 1, (height / 3) * 0, },
			{ (width / 4) * 1, (height / 3) * 2, },
			{ (width / 4) * 1, (height / 3) * 1, },
			{ (width / 4) * 3, (height / 3) * 1, },
		};

		memcpy(paddings, paddingscopy, sizeof(paddings));

	}
	
	if (data)
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			unsigned char *extractedData = new unsigned char[3 *
				(width / immageRatio.x) * (height / immageRatio.y)];

			int index = 0;

			int paddingX = paddings[i].x;
			int paddingY = paddings[i].y;

			for (int j = 0; j < height / immageRatio.y; j++)
				for (int i = 0; i < width / immageRatio.x; i++)
				{
					extractedData[index] = *getPixel(i + paddingX, j + paddingY, data);
					extractedData[index + 1] = *(getPixel(i + paddingX, j + paddingY, data) + 1);
					extractedData[index + 2] = *(getPixel(i + paddingX, j + paddingY, data) + 2);
					
					index += 3;
				}

			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width / immageRatio.x, height / immageRatio.y, 0,
				GL_RGB, GL_UNSIGNED_BYTE, extractedData
			);



			delete[] extractedData;
		}

		stbi_image_free(data);

	}
	else
	{
		throw(std::string("err loading ") + texture + "\n");
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
	//glDepthMask(GL_FALSE);
	glBindVertexArray(vao);
	shader.bind();
	
	glUniform1i(u_cubeMap, 0);
	glUniformMatrix4fv(u_viewProjection, 1, GL_FALSE, &viewProjection[0][0]);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glActiveTexture(GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	//glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

}

void SkyBox::clearTexture()
{
	glDeleteTextures(1, &cubemapTexture);
	cubemapTexture = 0;

}
