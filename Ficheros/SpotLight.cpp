#include "SpotLight.h"


SpotLight::SpotLight(GLfloat direccion[], GLfloat angulo, GLfloat exponente, glm::fvec3 pos)
{
	glLightf(id, GL_SPOT_CUTOFF, angulo);//establecemos valores del cono que forma el foco
	glLightf(id, GL_SPOT_EXPONENT, exponente);
	glLightfv(id, GL_SPOT_DIRECTION, direccion);
	setPos(pos);//establecemos posicion (luz puntual, ver Light.h)->la luz surge a partir de un punto en la direccion dada
}
