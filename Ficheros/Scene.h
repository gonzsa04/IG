//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <vector>
#include "Camera.h"
#include "Entities.h"
#include "SpotLight.h"
#include "DirLight.h"

//-------------------------------------------------------------------------

class Scene
{
public:
	Scene(Camera* cam) : camera(cam) { };
	~Scene();
	void init();
	void render();
	void update(GLuint timeElapsed);
	void RotateDiabolo(bool b) { rotate = b; }
	Texture textureFoto;
	void directionalOnOff(){
		directionalEnable = !directionalEnable;
		if (directionalEnable)directional->enable();
		else directional->disable();
	}
	void spotOnOff(){
		spotEnable = !spotEnable;
		if (spotEnable)spot->enable();
		else spot->disable();
	}
	void esferaLuzSpotOnOff() { esferaLuz->spotOnOff(); }
protected:
	Light* directional;
	SpotLight* spot;
	EsferaLuz* esferaLuz;
	Camera* camera;
	bool rotate = false;
	bool spotEnable = true, directionalEnable = true;
	std::vector<Entity*> objetos;
};

//-------------------------------------------------------------------------

#endif //_H_Scene_H_

