#include "IndexMesh.h"

void IndexMesh::enable() {
	Mesh::enable(); // glEnableClientState(…), glXXXPointer(…)
	if (indices != nullptr) {
		glEnableClientState(GL_INDEX_ARRAY);
		glIndexPointer(GL_UNSIGNED_INT, 0, indices);
	}
}
void IndexMesh::draw() {
	enable();
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);
	disable();
}
void IndexMesh::disable() {
	Mesh::disable(); // glDisableClientState(…)
	glDisableClientState(GL_INDEX_ARRAY);
}

//genera una malla
IndexMesh* IndexMesh::generateGrid(GLdouble lado, GLuint numDiv)
{ // grid cuadrado de lado*lado, centrado en el plano Y=0,
	// dividido en numDiv*numDiv celdas
	IndexMesh* m = new IndexMesh();
	GLdouble incr = lado / numDiv; // incremento para la coordenada x, y la z
	GLuint nVer = numDiv + 1;// num. de vértices por filas y por columnas
	// generar vértices
	m->numVertices = nVer * nVer; // num. de vértices
	m->vertices = new glm::dvec3[m->numVertices];
	m->texCoords = new glm::dvec2[m->numVertices];

	//recorremos el cuadrado de lado*lado
	GLfloat z = -lado / 2;
	for (int i = 0; i < nVer; i++){
		GLfloat x = -lado / 2;
		for (int j = 0; j < nVer; j++){
			//y asignamos el lugar de los vertices a la vez que sus coordenadas de textura
			m->vertices[i * nVer + j] = glm::dvec3(x, 0, z);
			x += incr;
			m->texCoords[i * nVer + j] = glm::dvec2((i)*1.0/numDiv ,(j)*1.0/numDiv );
		}
		z += incr;
	}

	// generar índices
	m->numIndices = numDiv * numDiv * 6;; // num. de índices
	m->indices = new GLuint[m->numIndices];

	//recorremos el cuadrado de lado*lado
	GLfloat iv;
	GLint k = 0;
	for (int i = 0; i < numDiv; i++) {
		for (int j = 0; j < numDiv; j++) {
			iv = i * nVer + j;
			//y creamos los triangulos de la malla a partir de los vertices (arriba izq, abajo izq, arriba der,
			//arriba der, abajo izq, abajo der -> dos triangulos por cuadrante)
			m->indices[k++] = iv;
			m->indices[k++] = iv + nVer;
			m->indices[k++] = iv + 1;
			m->indices[k++] = iv + 1;
			m->indices[k++] = iv + nVer;
			m->indices[k++] = iv + nVer + 1;
		}
	}

	return m;//devolvemos la malla
}

//genera el terreno modificando una malla a partir de un .raw
IndexMesh* IndexMesh::generateTerrain() {
	std::ifstream file("..//Bmps/terrain.raw", std::ios::binary); // open
	if (!file.is_open()) return nullptr; // "terrain.raw": 257*257 unsigned chars
	GLuint nDiv = 256; // nVer=257.
	GLdouble lado = nDiv * 8; // para incr=8
	IndexMesh* m = generateGrid(lado, nDiv);//generamos la malla
	GLuint nVer = nDiv + 1;
	unsigned char * data = new unsigned char[m->numVertices];
	file.read((char *)data, m->numVertices * sizeof(char));//guardamos en data los datos del .raw
	file.close(); // char = byte -> leer en data un bloque de numVertices bytes
	// modificar la coordenada Y de los vértices con los datos de data (*0.5)
	for (int i = 0; i < m->numVertices; i++) {
			m->vertices[i].y = data[i]*0.5;
	}
	// generar coordenadas de textura -> recorrido de vértices
	delete data;
	// generar normales -> recorrido de triángulos
	return m;
}