#pragma once

#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

class Light {
protected:
	static GLuint cont; // valor inicial cont = 0
	GLuint id = GL_MAX_LIGHTS; // GL_LIGHTi
	glm::fvec4 ambient = { 0.1, 0.1, 0.1, 1 };//intensidad de la luz
	glm::fvec4 diffuse = { 0.8, 0.8, 0.8, 1 };
	glm::fvec4 specular = { 0.5, 0.5, 0.5, 1 };
	glm::fvec4 posDir = { 0, 0, 1, 0 }; // w=0 -> vector, apuntando en z
public:
	Light() {
		if (cont < GL_MAX_LIGHTS) {
			id = GL_LIGHT0 + cont;
			++cont;
			glEnable(id);
		}
	};
	~Light() { disable(); }
	void disable() { if (id < GL_LIGHT0 + GL_MAX_LIGHTS) { glDisable(id); } };
	void enable() { if (id < GL_LIGHT0 + GL_MAX_LIGHTS) { glEnable(id); } }; 
	void setAmb(glm::fvec4 amb);
	virtual void load(glm::dmat4 const& modelViewMat) {
		glLightfv(id, GL_AMBIENT, value_ptr(ambient));
		glLightfv(id, GL_DIFFUSE, value_ptr(diffuse));
		glLightfv(id, GL_SPECULAR, value_ptr(specular));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(value_ptr(modelViewMat));
		glLightfv(id, GL_POSITION, value_ptr(posDir));
	};
	void setPos(glm::fvec3 pos){ posDir = glm::fvec4(pos, 1.0); };//luz puntual (el 1.0 indica que posDir es un punto)
	void setDir(glm::fvec3 dir){ posDir = glm::fvec4(dir, 0.0); }; //luz direccional (el 0.0 indica que posDir es un vector)
};

