#include "..\include\renderer.h"

void ChunksRenderer::init()
{

	texture.load(RESOURCES_PATH "test.jpg");

	//front
	float facePositions[6][12] =
	{
		//front
		0.5, 0.5, 0.5,
		-0.5, 0.5, 0.5,
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,

		//back
		-0.5, -0.5, -0.5,
		-0.5, 0.5, -0.5,
		0.5, 0.5, -0.5,
		0.5, -0.5, -0.5,

		//bottom
		-0.5, 0.5, -0.5,
		-0.5, 0.5, 0.5,
		0.5, 0.5, 0.5,
		0.5, 0.5, -0.5,

		//top
		0.5, -0.5, 0.5,
		-0.5, -0.5, 0.5,
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
	
		//left
		-0.5, -0.5, 0.5,
		-0.5, 0.5, 0.5,
		-0.5, 0.5, -0.5,
		-0.5, -0.5, -0.5,
	
		//right
		0.5, 0.5, -0.5,
		0.5, 0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, -0.5, -0.5,

	};

	//front
	float faceTexture[6][8] =
	{
		1,1,
		0,1,
		0,0,
		1,0,

		0,0,
		0,1,
		1,1,
		1,0,

		1,1,
		0,1,
		0,0,
		1,0,

		1,1,
		0,1,
		0,0,
		1,0,

		1,1,
		0,1,
		0,0,
		1,0,

		1,1,
		0,1,
		0,0,
		1,0,
	};

	//front
	unsigned int faceIndeces[6]
	{
		0,1,2,
		2,3,0
	};

	shader.load();

	glGenBuffers(1, &faceIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndeces), faceIndeces, GL_STATIC_DRAW);

	glGenVertexArrays(6, faceVAO);
	glGenBuffers(6, faceBuffer);
	glGenBuffers(6, faceTextureIndexesBuffer);


	for(int i=0; i<6;i++)
	{
		glBindVertexArray(faceVAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, faceBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(facePositions[i]), facePositions[i], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glBindBuffer(GL_ARRAY_BUFFER, faceTextureIndexesBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(faceTexture[i]), faceTexture[i], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIndexBuffer);

	}


	glBindVertexArray(0);

}

void ChunksRenderer::render(Camera c, glm::ivec3 pos)
{
	for(int i=0; i<6; i++)
	{
		glBindVertexArray(faceVAO[i]);
		shader.bind();
		shader.setPlayerPos(c.getPosition());
		shader.setProjectionMatrix(c.getProjectionMatrix());
		shader.setModelViewMatrix(c.getViewMatrix());
		shader.setPosition(pos.x, pos.y, pos.z);
		texture.bind(0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	}


	glBindVertexArray(0);

}
