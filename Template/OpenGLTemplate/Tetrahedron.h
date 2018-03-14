#pragma once
#include "Common.h"
#include "Texture.h"
#include "VertexBufferObject.h"
// Class for generating a tetrahedron
class Tetrahedron
{
public:
	Tetrahedron();
	~Tetrahedron();
	void Create();
	void Render();
	void Release();
private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;
};

