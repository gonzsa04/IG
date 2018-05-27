#pragma once
#include "Mesh.h"
#include <fstream>

//clase que hace mallas a partir de un .raw
class IndexMesh : public Mesh { // Mesh incorpora la tabla de normales, vertices y textura
protected:
	GLuint* indices; // para la tabla de índices de cada triangulo de la malla
	GLuint numIndices;//numero de indices: numero de triangulos en la malla*3
	virtual void enable();
	virtual void disable();
public:
	IndexMesh() {
		numIndices = 0; indices = nullptr;
		type = GL_TRIANGLES;//la malla se va a rellenar con triangulos
	}
	~IndexMesh() { delete[] indices; }
	virtual void draw();
	static IndexMesh* generateGrid(GLdouble lado, GLuint numDiv);
	static IndexMesh* generateTerrain();
};

