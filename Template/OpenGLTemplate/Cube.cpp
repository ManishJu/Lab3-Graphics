#include "Cube.h"

CCube::CCube()
{
	m_vao = NULL;
}

CCube::~CCube()
{
	Release();
}

void CCube::Create()
{
	m_texture.Load("resources//Textures//Crate.jpg");
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create();
	m_vbo.Bind();
	glm::vec2 texCoord1(0.0f, 0.0f);
	glm::vec2 texCoord2(0.0f, 1.0f);
	glm::vec2 texCoord3(1.0f, 0.0f);
	glm::vec2 texCoord4(1.0f, 1.0f);

	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	glm::vec3 normal2(0.0f, -1.0f, 0.0f);
	glm::vec3 normal3(0.0f, 0.0f, -1.0f);
	glm::vec3 normal4(0.0f, 0.0f, 1.0f);
	glm::vec3 normal5(1.0f, 0.0f, 0.0f);
	glm::vec3 normal6(-1.0f, 0.0f, 0.0f);
	
	{
		//front face
		m_vbo.AddData(&glm::vec3(-1, -1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord1, sizeof(glm::vec2));
		m_vbo.AddData(&normal6, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(-1, -1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord2, sizeof(glm::vec2));
		m_vbo.AddData(&normal6, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(-1, 1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord3, sizeof(glm::vec2));
		m_vbo.AddData(&normal6, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(-1, 1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord4, sizeof(glm::vec2));
		m_vbo.AddData(&normal6, sizeof(glm::vec3));
		//back face
		m_vbo.AddData(&glm::vec3(1, -1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord1, sizeof(glm::vec2));
		m_vbo.AddData(&normal5, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(1, -1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord2, sizeof(glm::vec2));
		m_vbo.AddData(&normal5, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(1, 1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord3, sizeof(glm::vec2));
		m_vbo.AddData(&normal5, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(1, 1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord4, sizeof(glm::vec2));
		m_vbo.AddData(&normal5, sizeof(glm::vec3));
		//left face
		m_vbo.AddData(&glm::vec3(1, -1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord1, sizeof(glm::vec2));
		m_vbo.AddData(&normal3, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(-1, -1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord2, sizeof(glm::vec2));
		m_vbo.AddData(&normal3, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(1, 1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord3, sizeof(glm::vec2));
		m_vbo.AddData(&normal3, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(-1, 1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord4, sizeof(glm::vec2));
		m_vbo.AddData(&normal3, sizeof(glm::vec3));

		//right face
		m_vbo.AddData(&glm::vec3(-1, -1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord1, sizeof(glm::vec2));
		m_vbo.AddData(&normal4, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(1, -1,1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord2, sizeof(glm::vec2));
		m_vbo.AddData(&normal4, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(-1, 1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord3, sizeof(glm::vec2));
		m_vbo.AddData(&normal4, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(1, 1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord4, sizeof(glm::vec2));
		m_vbo.AddData(&normal4, sizeof(glm::vec3));

		//up face
		m_vbo.AddData(&glm::vec3(-1, 1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord1, sizeof(glm::vec2));
		m_vbo.AddData(&normal, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(-1, 1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord2, sizeof(glm::vec2));
		m_vbo.AddData(&normal, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(1, 1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord3, sizeof(glm::vec2));
		m_vbo.AddData(&normal, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(1, 1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord4, sizeof(glm::vec2));
		m_vbo.AddData(&normal, sizeof(glm::vec3));

		//down face
		m_vbo.AddData(&glm::vec3(1, -1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord1, sizeof(glm::vec2));
		m_vbo.AddData(&normal2, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(1, -1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord2, sizeof(glm::vec2));
		m_vbo.AddData(&normal2, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(-1, -1, -1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord3, sizeof(glm::vec2));
		m_vbo.AddData(&normal2, sizeof(glm::vec3));
		m_vbo.AddData(&glm::vec3(-1, -1, 1), sizeof(glm::vec3));
		m_vbo.AddData(&texCoord4, sizeof(glm::vec2));
		m_vbo.AddData(&normal2, sizeof(glm::vec3));

	}
	// Upload data to GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}

void CCube::Render()
{
	glBindVertexArray(m_vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_texture.Bind();
	// Call glDrawArrays to render each side
	glDrawArrays(GL_TRIANGLE_STRIP,0, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 8, 12);


}

void CCube::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}


