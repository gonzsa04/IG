#include "Scene.h"
#include <gtc/matrix_transform.hpp>  

//-------------------------------------------------------------------------

void Scene::init()
{ // OpenGL basic setting
  glClearColor(1.0, 1.0, 1.0, 1.0);  // background color (alpha=1 -> opaque)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);//activamos el uso de texturas
  glEnable(GL_LIGHTING);//Activa la iluminación de OpenGL 
  glEnable(GL_NORMALIZE); //Activa la normalización de vectores
  glShadeModel(GL_SMOOTH);// Gouraud Shading

  camera->setAZ();
    
  // lights
  GLfloat direccion[] = { 0.0, 0.0, -1.0 };//direccion a la que apuntara el foco
  directional = new DirLight(glm::fvec3(0, 0, 1));//luz direccional apuntando en el eje z
  spot = new SpotLight(direccion, 30.0, 10.0, camera->getEye());//foco que apunta en una direccion, 
  //con un angulo de apertura, un exponente de dispersion y una posicion

  //materiales de los planetas (cada uno uno distinto)
  Material mat1 = Material(glm::fvec4(0.0,0.0,0.0,1), glm::fvec4(0.0, 0.0, 0.0, 1), glm::fvec4(0.0, 0.0, 0.0, 1), 8.0);
  Material mat2 = Material(glm::fvec4(1.0, 1.0, 1.0, 1), glm::fvec4(1.0, 1.0, 1.0, 1), glm::fvec4(1.0, 1.0, 1.0, 1), 4.0);
  Material mat3 = Material(glm::fvec4(2.0, 2.0, 2.0, 1), glm::fvec4(2.0, 2.0, 2.0, 1), glm::fvec4(2.0, 2.0, 2.0, 1), 2.0);
  Material mat4 = Material(glm::fvec4(3.0, 3.0, 3.0, 1), glm::fvec4(3.0, 3.0, 3.0, 1), glm::fvec4(3.0, 3.0, 3.0, 1), 0.0);
  //planetas
  glm::dmat4 matPos(1.0);
  glm::dmat4 auxPos(1.0);
  Entity* esfera1 = new Esfera(100, "..//Bmps/venus.bmp");//esfera de radio 149
  matPos = translate(auxPos, glm::dvec3(300.0, 200.0, 50.0));
  esfera1->setModelMat(matPos);//le asignamos la posicion
  esfera1->setMaterial(mat1);//y el material
  Entity* esfera2 = new Esfera(80, "..//Bmps/mars.bmp");
  matPos = translate(auxPos, glm::dvec3(-100.0, -200.0, -20.0));
  esfera2->setModelMat(matPos);
  esfera2->setMaterial(mat2);
  Entity* esfera3 = new Esfera(60, "..//Bmps/moon.bmp");
  esfera3->setMaterial(mat3);
  esferaLuz = new EsferaLuz(150, "..//Bmps/sun.bmp");//esfera con foco apuntando hacia abajo
  matPos = translate(auxPos, glm::dvec3(-100.0, 200.0, -20.0));
  esferaLuz->setModelMat(matPos);
  esferaLuz->setMaterial(mat4);
  
  // objets
  //objetos.push_back(new EjesRGB(300.0));
  objetos.push_back(esfera1);
  objetos.push_back(esfera2);
  objetos.push_back(esfera3);
  objetos.push_back(esferaLuz);
  //objetos.push_back(new ContRectangle(500.0, 500.0, "..//Bmps/baldosaC.bmp", 5, 5));
  //objetos.push_back(new Foto(100.0, 100.0, 1, 1));
  //objetos.push_back(new ContCubo(75.0, "..//Bmps/container.bmp"));
  //objetos.push_back(new TriangleRGB(50.0));
  //objetos.push_back(new Triangle(75.0));
  //objetos.push_back(new TriPyramid(75.0, 75.0, "..//Bmps/emopng.bmp"));
  //objetos.push_back(new Diabolo(25, 50, "..//Bmps/floris.bmp"));
  //objetos.push_back(new Cubo(50.0, "..//Bmps/container.bmp", "..//Bmps/chuches.bmp"));
  //objetos.push_back(new Grass(100.0, "..//Bmps/grass.bmp"));//pintamos antes los objetos opacos que los translucidos (GlassPot)
  //objetos.push_back(new GlassPot(100.0, "..//Bmps/window.bmp"));
  //objetos.push_back(new Dragon(3000));//clase dragon hecha (opcional)
  //objetos.push_back(new PoliEspiral(0.0, 300.0, 1.0, 1.0, 100.0));
}
//-------------------------------------------------------------------------

Scene::~Scene()
{ // free memory and resources 
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);//desactivamos el uso de texturas
	glEnable(GL_LIGHTING);//desactiva la iluminación de OpenGL
	glDisable(GL_NORMALIZE); //desactiva la normalización de vectores

	for each (Entity* it in objetos)
	{
		delete it;
	}
}
//-------------------------------------------------------------------------

void Scene::render()
{
  glMatrixMode(GL_MODELVIEW);
  directional->load(camera->getViewMat());//actualiza graficamente las luces (load=render + o -)
  spot->setDir(camera->getFront());//actualizamos la direccion a la que apunta el foco con donde apunta la camara
  spot->setPos(camera->getEye());//actualizamos la posicion del foco con la posicion de la camara
  spot->load(camera->getViewMat());//esta actualizacion de luces hay que hacerla antes de los demas render
	for each (Entity* it in objetos) {
		it->render(camera->getViewMat());
	}

}
//-------------------------------------------------------------------------

void Scene::update(GLuint timeElapsed){
	for each (Entity* it in objetos){
		it->update(timeElapsed);
	}
}

