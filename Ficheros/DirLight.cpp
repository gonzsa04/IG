#include "DirLight.h"

DirLight::DirLight(glm::fvec3 direccion)
{
	Light::Light();//se llama a la constructora de luz
	setDir(direccion);//establecemos direccion (luz direccional, ver Light.h)->la luz siempre va en esa direccion de forma paralela
}
