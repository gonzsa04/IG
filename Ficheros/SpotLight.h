#pragma once
#include "Light.h"

//luz de foco, es una fuente de luz posicional
class SpotLight:public Light{
public:
	//la constructora recibe direccion, angulo de apertura, exponente de dispersion y posicion
	SpotLight() {}
	SpotLight(GLfloat direccion[], GLfloat angulo, GLfloat exponente, glm::fvec3 pos);
	virtual ~SpotLight() {}
};

