//#pragma once
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
   GLuint numVertices = 0;
   GLuint type = GL_POINTS;
   glm::dvec3* vertices = nullptr;
   glm::dvec4* colors = nullptr;
   glm::dvec2 * texCoords; // array de coordenadas de textura
};

//-------------------------------------------------------------------------

#endif //_H_Scene_H_