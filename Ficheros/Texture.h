#pragma once
#include <string>
#include "Pixmap32RGBA.h"
#include <glm.hpp>

class Texture // utiliza la clase PixMap32RGBA para el método load
{
public:
	Texture() : w(0), h(0), id(0) {};
	~Texture() { if (id != 0) glDeleteTextures(1, &id); };
	bool load(const std::string & BMP_Name, GLubyte alpha = 255, glm::ivec3 color = { -1, -1, -1 });
	// cargar y transferir a openGL
	void unbind() { glBindTexture(GL_TEXTURE_2D, 0); };//desactiva la textura en la GPU
	void loadColorBuffer(GLsizei width, GLsizei height);
	void save(const std::string & BMP_Name);
	//segun init, en caso de que las coord de textura queden fuera de [0, 1] la textura se repite. Con este metodo se cambia
	void wrap() {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	void bind(GLuint mix = GL_REPLACE); // posibles valores: GL_MODULATE, GL_ADD, .. 
protected:
	GLuint w, h; // dimensiones de la imagen
	GLuint id; // identificador interno (GPU) de la textura
	void init();
};

