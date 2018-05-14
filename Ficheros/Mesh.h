#pragma once
#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/freeglut.h>
#include <glm.hpp>

//-------------------------------------------------------------------------

class Mesh
{
public:
	static Mesh* generateAxesRGB(GLdouble l);
	static Mesh* generateTriangle(GLdouble r);
	static Mesh* generateTriangleRGB(GLdouble r);
	static Mesh* generateTriPyramid(GLdouble r, GLdouble h);
	static Mesh* generateContCubo(GLdouble l);
	static Mesh* generaDragon(GLuint numVert);
	static Mesh* generateRectangle(GLdouble w, GLdouble h);
	static Mesh* generateCuadrado(GLdouble l);
	static Mesh* generaPoliespiral(GLdouble angIni, GLdouble incrAng, GLdouble ladoIni, GLdouble incrLado, GLuint numVert);
	static Mesh* generateRectangleTex(GLdouble w, GLdouble h, GLuint x, GLuint y);
	static Mesh* generateTriPyramidTex(GLdouble r, GLdouble h);
	static Mesh* generateContCubeTex(GLdouble l);

	Mesh(void) { };
	~Mesh(void);
	virtual void draw();
	glm::dvec3* getVertices() { return vertices; };
	glm::dvec4* getColours() { return colors; };

protected:
	void enable(){
		if (vertices != nullptr) {
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glVertexPointer(3, GL_DOUBLE, 0, vertices);  // number of coordinates per vertex, type of each coordinate 
			glNormalPointer(GL_DOUBLE, 0, normals);
			if (colors != nullptr) {
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(4, GL_DOUBLE, 0, colors);   // number of coordinates per color, type of each coordinate 
			}
			if (texCoords != nullptr){//si vamos a pintar una textura
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);//activamos el array de coordenadas de la textura (texCoords)
				glTexCoordPointer(2, GL_DOUBLE, 0, texCoords);
			}
		}
	}

	void disable(){
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	GLuint numVertices = 0;
	GLuint type = GL_POINTS;
	glm::dvec3* vertices = nullptr;
	glm::dvec4* colors = nullptr;
	glm::dvec2 * texCoords; // array de coordenadas de textura
	glm::dvec3* normals = nullptr;//vectores normales
};

//-------------------------------------------------------------------------

#endif //_H_Mesh_H_