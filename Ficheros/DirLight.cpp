#include "DirLight.h"

//no tiene posicion concreta como la luz de foco
DirLight::DirLight(glm::fvec3 direccion)
{
	setDir(direccion);//establecemos direccion (luz direccional, ver Light.h)->la luz siempre va en esa direccion de forma paralela
}
