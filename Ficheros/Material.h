#pragma once

#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

class Material {
private:
	glm::fvec4 ambient, diffuse, specular; // coeficientes de reflexión
	GLfloat expF; // exponente especular
	GLuint face = GL_FRONT_AND_BACK; // ambos lados
	GLuint sh = GL_SMOOTH; // smooth shading
public:
	Material() {};
	Material(glm::fvec4 ambiente, glm::fvec4 difusion, glm::fvec4 especular, GLfloat exponene, GLuint cara = GL_FRONT_AND_BACK, GLuint sombreadito = GL_SMOOTH): 
		ambient(ambiente), diffuse(difusion), specular(especular), expF(exponene), face(cara), sh(sombreadito){};
	~Material(){}
	virtual void load() {
		glMaterialfv(face, GL_AMBIENT, value_ptr(ambient));
		glShadeModel(sh);
	}
};
