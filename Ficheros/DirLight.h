#pragma once
#include "Light.h"

//luz en la que sus rayos son paralelos
class DirLight : public Light {
public:
	DirLight(glm::fvec3 direccion);//la constructora recibe la direccion en la que se quiere que apunte la luz
	virtual ~DirLight() {}
};

