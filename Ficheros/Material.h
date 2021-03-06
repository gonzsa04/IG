#pragma once

#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

class Material {
private:
	glm::fvec4 ambient, diffuse, specular; // coeficientes de reflexi�n
	GLfloat expF; // exponente especular
	GLuint face = GL_FRONT_AND_BACK; // el material se pone por ambos lados
	GLuint sh = GL_SMOOTH; // smooth shading
public:
	Material() {};
	Material(glm::fvec4 ambiente, glm::fvec4 difusion, glm::fvec4 especular, GLfloat exponene, GLuint cara = GL_FRONT_AND_BACK, GLuint sombreadito = GL_SMOOTH): 
		ambient(ambiente), diffuse(difusion), specular(especular), expF(exponene), face(cara), sh(sombreadito){};
	~Material(){}
	virtual void load() {
		//establecemos coeficientes de reflexion
		glMaterialfv(face, GL_AMBIENT, value_ptr(ambient));
		glMaterialfv(face, GL_AMBIENT, value_ptr(diffuse));
		glMaterialfv(face, GL_AMBIENT, value_ptr(specular));
		glMaterialf(face, GL_SHININESS, expF);
		glShadeModel(sh);
	}
};
