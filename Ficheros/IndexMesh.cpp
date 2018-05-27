#include "IndexMesh.h"

void IndexMesh::enable() {
	Mesh::enable(); // glEnableClientState(…), glXXXPointer(…)
	if (indices != nullptr) {
		glEnableClientState(GL_INDEX_ARRAY);//activamos el array de indices
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
	glDisableClientState(GL_INDEX_ARRAY);//desactivamos el array de indices
}

//genera una malla plana
IndexMesh* IndexMesh::generateGrid(GLdouble lado, GLuint numDiv)
{ // grid cuadrado de lado*lado, centrado en el plano Y=0,
	// dividido en numDiv*numDiv celdas
	IndexMesh* m = new IndexMesh();
	GLdouble incr = lado / numDiv; // incremento para la coordenada x, y la z
	GLuint nVer = numDiv + 1;// num. de vértices por filas y por columnas
	// generar vértices
	m->numVertices = nVer * nVer; // num. de vértices
	//las tablas de datos tienen que ser del mismo tamaño (numVertices)
	m->vertices = new glm::dvec3[m->numVertices];//coord de cada vertice (posicion)
	m->texCoords = new glm::dvec2[m->numVertices];//coord de textura de cada vertice
	m->normals = new glm::dvec3[m->numVertices];//coord de vectores normales a cada vertice (orientacion)

	//recorremos el cuadrado de lado*lado
	GLfloat z = -lado / 2;
	for (int i = 0; i < nVer; i++){
		GLfloat x = -lado / 2;
		for (int j = 0; j < nVer; j++){
			//y asignamos el lugar de cada vertice a la vez que su coordenada de textura y su normal
			m->vertices[i * nVer + j] = glm::dvec3(x, 0, z);//posicion
			x += incr;
			m->texCoords[i * nVer + j] = glm::dvec2((i)*1.0/numDiv ,(j)*1.0/numDiv );//textura
			m->normals[i * nVer + j] = glm::dvec3(0.0);//incializamos normales a 0
		}
		z += incr;
	}

	// generar índices
	m->numIndices = numDiv * numDiv * 6; // num. de índices (en cada cuadrado hay dos triangulos, 6 vertices)
	m->indices = new GLuint[m->numIndices];//indices de cada triangulo de la malla

	//recorremos el cuadrado de lado*lado
	GLfloat iv;
	GLint k = 0;
	for (int i = 0; i < numDiv; i++) {
		for (int j = 0; j < numDiv; j++) {
			iv = i * nVer + j;
			//y creamos los triangulos de la malla a partir de los indices (arriba izq, abajo izq, arriba der,
			//arriba der, abajo izq, abajo der -> dos triangulos por cuadrante) deben darse en el sentido contrario de las agujas del reloj
			//asi sus normales despues apuntaran hacia afuera
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

//genera el terreno creando una malla a partir de un .raw
IndexMesh* IndexMesh::generateTerrain() {
	std::ifstream file("..//Bmps/terrain.raw", std::ios::binary); // open
	if (!file.is_open()) return nullptr; // "terrain.raw": 257*257 unsigned chars
	GLuint nDiv = 256; // nVer=257.
	GLdouble lado = nDiv * 8; // para incr=8
	IndexMesh* m = generateGrid(lado, nDiv);//generamos la malla, por el momento sera plana, con las dimensiones y divisiones especificadas
	GLuint nVer = nDiv + 1;
	unsigned char * data = new unsigned char[m->numVertices];//data guardara la informacion de altura de cada vertice
	file.read((char *)data, m->numVertices * sizeof(char));//guardamos en data los datos del .raw
	file.close(); // char = byte -> leer en data un bloque de numVertices bytes
	// modificar la coordenada Y de los vértices con los datos de data (*0.5)
	for (int i = 0; i < m->numVertices; i++) {
			m->vertices[i].y = data[i]*0.5;//le eplicamos la altura guardada en data (relieve de la malla)
	}
	delete data;

	// generar normales -> recorrido de triángulos
	int numIndices = nDiv * nDiv * 6;
	glm::dvec3 a, b, c, n;
	//damos valor a las normales
	//recorremos de tres en tres, para ir de triangulo en triangulo
	for (int i = 0; i < numIndices; i+=3) {
		//para cada triangulo
		a = m->vertices[m->indices[i]]; //a guarda la posicion (m->vertices) del primer indice (vertice) de ese triangulo
		b = m->vertices[m->indices[i + 1]];//b guarda la del segundo
		c = m->vertices[m->indices[i + 2]];//c la del tercero
		n = cross(b - a, c - a);//n guarda el vector normal al triangulo (a, b, c)->producto vectorial de (c-b)x(a-b)
		m->normals[m->indices[i]] += n;//guardamos en normales el vector normal del triangulo (a, b, c)
		m->normals[m->indices[i + 1]] += n;//cada una de las tres posiciones tendran el mismo vecor normal
		m->normals[m->indices[i + 2]] += n;
	}
	
	//y las normalizamos
	for (int i = 0; i < m->numVertices; i++){
		m->normals[i] = normalize(m->normals[i]);
	}

	return m;
}