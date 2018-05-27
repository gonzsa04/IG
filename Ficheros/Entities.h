//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "SpotLight.h"
#include "IndexMesh.h"

//-------------------------------------------------------------------------

class Entity 
{
public:
	Entity() :modelMat(1.0), texture(nullptr) {}//sin nada -> no textura
	Entity(const std::string & BMP_Name) : modelMat(1.0) { //con nombre de textura
		texture = new Texture();
		texture->load(BMP_Name); //se carga en la textura esa direccion
	};
  virtual ~Entity() { 
	  delete mesh;
	  delete texture;
	  texture = nullptr;
  };

  virtual void render(glm::dmat4 const& modelViewMat);
  virtual void update(GLuint timeElapsed) {}
  virtual void setModelMat(glm::dmat4 const& mMat) { modelMat = mMat; }
  void setMaterial(Material const& mt) { material = mt; }
  void setTexture(Texture tex) { texture = &tex; };
  
protected:
  Mesh* mesh = nullptr;
  Material material;
  Texture* texture;
  glm::dmat4 modelMat;
  virtual void draw();
  virtual void setMvM(glm::dmat4 const& modelViewMat);
};

//-------------------------------------------------------------------------

class EjesRGB : public Entity 
{
public:
  EjesRGB(GLdouble l);
  ~EjesRGB() { };
  virtual void draw();
};

class Triangle : public Entity{
public:
	Triangle(GLdouble r);
	~Triangle(){};
	virtual void draw();
};

class TriangleRGB : public Entity{
public:
	TriangleRGB(GLdouble r);
	~TriangleRGB(){};
	virtual void draw();
};

class TriPyramid : public Entity{
public:
	TriPyramid(GLdouble r, GLdouble h);
	TriPyramid(GLdouble r, GLdouble h, const std::string & BMP_Name);
	~TriPyramid(){};
	virtual void draw();
};

class ContCubo : public Entity{
public:
	ContCubo(GLdouble l);
	ContCubo(GLdouble l, const std::string & BMP_Name);
	~ContCubo(){};
	virtual void draw();
};

class Diabolo : public Entity{
private:
	GLdouble radio, altura, rotation = 1.0;
public:
	Diabolo(GLdouble r, GLdouble h);
	Diabolo(GLdouble r, GLdouble h, const std::string & BMP_Name);
	~Diabolo(){};
	virtual void update(GLuint timeElapsed) {
		setRotation(glm::mod(rotation + 0.01 * timeElapsed, 360.0));
	}
	virtual void draw();
	virtual void render(glm::dmat4 const& modelViewMat);
	void setRotation(GLdouble newRot) { rotation += newRot; }
};

class Cubo : public Entity {
private:
	GLdouble l;
	Texture* textureFondo = nullptr;
public:
	Cubo(GLdouble lado);
	Cubo(GLdouble l, const std::string & BMP_Name, const std::string & BMP_Fondo);
	~Cubo() { delete textureFondo; textureFondo = nullptr; };
	virtual void draw();
	virtual void render(glm::dmat4 const& modelViewMat);
};

class ContRectangle : public Entity {
public:
	ContRectangle(GLdouble w, GLdouble h);
	ContRectangle(GLdouble w, GLdouble h, const std::string & BMP_Name, GLuint x, GLuint y);
	~ContRectangle() {};
	virtual void draw();
	virtual void render(glm::dmat4 const& modelViewMat);
};

//clase dragon hecha (opcional)
class Dragon : public Entity {
private:
	GLdouble numVert;
public:
	Dragon(GLuint numVert);
	~Dragon() {};
	virtual void draw();
	virtual void render(glm::dmat4 const& modelViewMat);
};

//clase poliespiral hecha (opcional)
class PoliEspiral : public Entity {
public:
	PoliEspiral(GLdouble angIni, GLdouble incrAng, GLdouble ladoIni, GLdouble incrLado, GLuint numVert);
	~PoliEspiral() {};
	virtual void draw();
};

//foto puesta en el suelo que muestra lo que hay en pantalla
class Foto : public Entity {
public:
	Foto(GLdouble w, GLdouble h, GLuint x, GLuint y);
	~Foto() {};
	virtual void update(GLuint timeElapsed){//cada vez que llama a update cargamos en la textura el Color Buffer actual (lo que hay en pantalla)
		texture->loadColorBuffer(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
	virtual void draw();
	virtual void render(glm::dmat4 const& modelViewMat);
};

//mampara donde esta la planta
class GlassPot :public Entity {
private:
	GLdouble l;
public:
	GlassPot(GLdouble l, const std::string & BMP_Name);
	~GlassPot() {};
	virtual void draw();
	virtual void render(glm::dmat4 const& modelViewMat);
};

//planta
class Grass :public Entity {
private:
	GLdouble l;
public:
	Grass(GLdouble l, const std::string & BMP_Name);
	~Grass() {};
	virtual void draw();
	virtual void render(glm::dmat4 const& modelViewMat);
};

//clase que genera una esfera (para los planetas)
class Esfera :public Entity {
protected:
	GLdouble r;//radio
	GLUquadricObj *quadric;//objeto cuadrico que reproduce una figura (en este caso esfera)
public:
	Esfera(GLdouble r, const std::string & BMP_Name);
	Esfera(GLdouble r);
	~Esfera() { gluDeleteQuadric(quadric); };
	virtual void draw();
};

//igual que la anterior, pero con una luz de foco dentro
class EsferaLuz : public Esfera {
private:
	SpotLight spot;//luz de foco
	bool spotEnable = true;
	Esfera *esfera1, *esfera2;//las dos esferas hijas
	glm::dvec3 posHija;//posicion de las hijas
	const double Cx = 512;
	const double Cy = 100;//altura a la que se encuentra el objeto
	const double Cz = -Cx;
	double ang = 0;
	GLdouble rotation = 1.0;
	void renderHijos(glm::dmat4 modelViewMat);//se encarga de pintar a las hijas
public:
	EsferaLuz(GLdouble r, const std::string & BMP_Name);
	EsferaLuz(GLdouble r);
	virtual ~EsferaLuz(){
		delete esfera1;
		delete esfera2;
	};
	virtual void render(glm::dmat4 const& modelViewMat);
	virtual void setModelMat(glm::dmat4 const& mMat) { 
		Entity::setModelMat(mMat);
		spot.setPos(mMat[3]);//al establecer la posicion de la esfera, se actualiza tambien la del foco para que este la siga
	}
	void spotOnOff() {
		spotEnable = !spotEnable;
		if (spotEnable)spot.enable();
		else spot.disable();
	}
	virtual void update(GLuint timeElapsed) {
		rotation = rotation + 0.1 * timeElapsed;//rotacion sobre el eje y
		ang = ang + 0.001 * timeElapsed;//angulo para hacer la trayectoria descrita
	}
};

//una clase que genera un terreno y le añade una textura o no
class Terreno : public Entity {
public:
	Terreno() { mesh = IndexMesh::generateTerrain(); };
	Terreno(const std::string & BMP_Name) : Entity(BMP_Name) { mesh = IndexMesh::generateTerrain(); };
	~Terreno() {};
	virtual void draw();
};

//-------------------------------------------------------------------------

#endif //_H_Entities_H_