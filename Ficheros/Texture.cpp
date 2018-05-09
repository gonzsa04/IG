#include "Texture.h"

void Texture::init() {
	glGenTextures(1, &id); // genera un identificador para una nueva textura
	glBindTexture(GL_TEXTURE_2D, id); 
	// Filters and clamping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//en caso de que la textura se agrande, el color de cada pixel sera la suma de los 4 de alrededor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//en caso de que la textura se minimice, el color de cada pixel sera la suma de los 4 de alrededor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//en caso de que las coord de textura queden fuera de [0, 1], la textura se repite
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::bind(GLuint mix) { // argumento para el modo de mezclar los colores
	glBindTexture(GL_TEXTURE_2D, id); // activa la textura
	// la función de mezcla de colores no queda guardada en el objeto
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mix);//reemplaza el color que haya por la textura
	// GL_MODULATE / GL_ADD … -> sumarian el color que haya con la textura
}

//nombre de la textura, alpha de la textura (por defecto a 255) y color sobre el que se aplica el apha (por defecto a un color no valido)
bool Texture::load(const std::string & BMP_Name, GLubyte alpha, glm::ivec3 color) {
	if (id == 0) init();
	PixMap32RGBA pixMap; // var. local para cargar la imagen del archivo
	pixMap.load_bmp24BGR(BMP_Name); // carga y añade alpha
								// carga correcta?
	//si el color es valido aplicamos el alpha solo sobre el
	if (color.x != -1) {
		PixMap32RGBA::rgba_color col;
		col.r = color.x;
		col.g = color.y;
		col.b = color.z;
		if (alpha != 255) pixMap.set_colorkey_alpha(col, alpha);
	}
	else if (alpha != 255) pixMap.set_alpha(alpha);//si no, sobre toda la textura

	w = pixMap.width();
	h = pixMap.height();
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, pixMap.data());
	// transferir a openGL
	return true;
}

void Texture::loadColorBuffer(GLsizei width, GLsizei height) {
	if (id == 0) init();
	w = width;
	h = height;
	glReadBuffer(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, id);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);//copia en la textura activa parte de la imagen del Color Buffer
	                                      //xLeft, yBottom, w, h, border
}

void Texture::save(const std::string & BMP_Name) {
	PixMap32RGBA buffer;//array donde guardar los datos (de tipo y tamaño adecuado)
	buffer.create_pixmap(w, h);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());//obtiene la imagen de la textura activa y lo guarda en buffer
	buffer.save_bmp24BGR(BMP_Name);//guarda la informacion de buffer con un nombre
}
