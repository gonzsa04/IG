#include "Entities.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;

//-------------------------------------------------------------------------

void Entity::render(dmat4 const& modelViewMat) 
{ 
	if (texture != nullptr)texture->bind();//si hay textura la activamos
	setMvM(modelViewMat); 
	draw();
	if (texture != nullptr)texture->unbind();//y la desactivamos despues de dibujar (draw)
}
//-------------------------------------------------------------------------

void Entity::draw() 
{ 
	if (mesh != nullptr) {
		material.load();//se carga el material antes de dibujar la malla
		mesh->draw();
	}
}
//-------------------------------------------------------------------------

void Entity::setMvM(dmat4 const& modelViewMat)
{
	glMatrixMode(GL_MODELVIEW);
	dmat4 aMat = modelViewMat * modelMat;
	glLoadMatrixd(value_ptr(aMat));
}
//-------------------------------------------------------------------------

EjesRGB::EjesRGB(GLdouble l): Entity() 
{
  mesh = Mesh::generateAxesRGB(l);
}

Triangle::Triangle(GLdouble r){
	mesh = Mesh::generateTriangle(r);
}

TriangleRGB::TriangleRGB(GLdouble r){
	mesh = Mesh::generateTriangleRGB(r);
}

TriPyramid::TriPyramid(GLdouble r, GLdouble h){
	mesh = Mesh::generateTriPyramid(r, h);
}

TriPyramid::TriPyramid(GLdouble r, GLdouble h, const std::string & BMP_Name):Entity(BMP_Name){
	mesh = Mesh::generateTriPyramidTex(r, h);
}

//si la constructora no recibe nombre de textura se genera un cubo normal
ContCubo::ContCubo(GLdouble l){
	mesh = Mesh::generateContCubo(l);
}

//si la constructora recibe nombre de textura se carga la textura (Entity(...)) y se genera un cubo con textura
ContCubo::ContCubo(GLdouble l, const std::string & BMP_Name) : Entity(BMP_Name) {
	mesh = Mesh::generateContCubeTex(l);
}

//igual con las demas clases
Diabolo::Diabolo(GLdouble r, GLdouble h) :radio(r), altura(h) {}

Diabolo::Diabolo(GLdouble r, GLdouble h, const std::string & BMP_Name) : radio(r), altura(h), Entity(BMP_Name) {}

Cubo::Cubo(GLdouble lado) : l(lado) {}

Cubo::Cubo(GLdouble lado, const std::string & BMP_Name, const std::string & BMP_Fondo) : l(lado), Entity(BMP_Name) {
	textureFondo = new Texture();//segunda textura para el fondo
	textureFondo->load(BMP_Fondo);
}

GlassPot::GlassPot(GLdouble lado, const std::string & BMP_Name) : l(lado) {
	texture = new Texture();
	texture->load(BMP_Name, 255/2); //se carga con menos alpha para que sea translucido
}

Grass::Grass(GLdouble lado, const std::string & BMP_Name) : l(lado) {
	texture = new Texture();
	texture->load(BMP_Name, 0, { 0, 0, 0 }); //se carga con alpha = 0 para el color 0, 0, 0 (negro), asi hacemos transparente el fondo
}

Esfera::Esfera(GLdouble radio, const std::string & BMP_Name) : Entity(BMP_Name), r(radio), quadric(gluNewQuadric()) {}

Esfera::Esfera(GLdouble radio) : r(radio), quadric(gluNewQuadric()) {}

EsferaLuz::EsferaLuz(GLdouble radio, const std::string & BMP_Name) : Esfera(radio, BMP_Name)
{
	GLfloat dir[] = { 0, -1, 0 };//direccion a la que apunta el foco de dentro
	spot = SpotLight(dir, 45.0, 4.0, modelMat[3]);//el foco con la direccion, el angulo, el exponente y la pos de la esfera (para que este en su centro)
	spot.enable();//la encendemos
	esfera1 = new Esfera(radio / 3, BMP_Name);//inicializamos a las hijas
	esfera2 = new Esfera(radio / 3, BMP_Name);
	Material mat = Material(glm::fvec4(3.0, 3.0, 3.0, 1), glm::fvec4(3.0, 3.0, 3.0, 1), glm::fvec4(0.0, 0.0, 0.0, 0), 0.0);
	esfera1->setMaterial(mat);//les añadimos material
	esfera2->setMaterial(mat);
	glm::dmat4 auxPos(1.0);
	auxPos = modelMat;
	posHija = { radio * 1.5, 0.0, 0.0 };//las situamos una a cada lado del padre
	esfera1->setModelMat(translate(auxPos, posHija));
	esfera2->setModelMat(translate(auxPos, -posHija));
}

EsferaLuz::EsferaLuz(GLdouble radio) : Esfera(radio)
{
	GLfloat dir[] = { 0, -1, 0 };
	spot = SpotLight(dir, 45.0, 4.0, modelMat[3]);
	spot.enable();
}

ContRectangle::ContRectangle(GLdouble w, GLdouble h) {
	mesh = Mesh::generateRectangle(w, h);
}

ContRectangle::ContRectangle(GLdouble w, GLdouble h, const std::string & BMP_Name, GLuint x, GLuint y) :Entity(BMP_Name) {
	mesh = Mesh::generateRectangleTex(w, h, x, y);
}

Dragon::Dragon(GLuint numVert):numVert(numVert) {}

PoliEspiral::PoliEspiral(GLdouble angIni, GLdouble incrAng, GLdouble ladoIni, GLdouble incrLado, GLuint numVert) {
	mesh = Mesh::generaPoliespiral(angIni, incrAng, ladoIni, incrLado, numVert);
}

//-------------------------------------------------------------------------

void EjesRGB::draw()
{
  glLineWidth(2);
  mesh->draw();
  glLineWidth(1);
}

void Triangle::draw()
{
	glLineWidth(2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mesh->draw();
	glLineWidth(1);
}

void TriangleRGB::draw()
{
	glLineWidth(2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mesh->draw();
	glLineWidth(1);
}

void TriPyramid::draw()
{
	if (texture != nullptr){
		texture->bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->draw();
		texture->unbind();
	}
	else{
		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mesh->draw();
		glLineWidth(1);
	}
}

void ContCubo::draw()
{
	if (texture != nullptr){//si tiene textura
		texture->bind();//la activamos
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->draw();//dibujamos
		texture->unbind();//la desactivamos
	}
	else{//si no pinta como normalmente
		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mesh->draw();
		glLineWidth(1);
	}
}

//igual con el resto de clases
void Diabolo::draw(){
	if (texture != nullptr) {
		texture->bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->draw();
		texture->unbind();
	}
	else {
		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mesh->draw();
		glLineWidth(1);
	}
}

void Cubo::draw() {
	if (texture != nullptr){//si tiene textura la pintamos por fuera
		texture->bind();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		mesh->draw();
		texture->unbind();
		if (textureFondo != nullptr) {//si ademas tiene textura de fondo la pintamos por dentro
			textureFondo->bind();
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			mesh->draw();
			textureFondo->unbind();
		}
		else {//si no tiene pintamos la textura de siempre tambien por dentro
			texture->bind();
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			mesh->draw();
			texture->unbind();
		}
		glDisable(GL_CULL_FACE);
	}
	else{
		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mesh->draw();
		glLineWidth(1);
	}
}

void GlassPot::draw() {
	if (texture != nullptr) {
		glEnable(GL_BLEND);//activamos blending para que haga las transparencias
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);//para que se vean los objetos de detras (la planta)
		texture->bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->draw();
		texture->unbind();
		glDepthMask(GL_TRUE);//para que dejen de verse los que tiene detras
		glDisable(GL_BLEND);//desactivamos blending
	}
}

void Grass::draw() {
	if (texture != nullptr) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		texture->bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->draw();
		texture->unbind();
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

void Esfera::draw() {
	material.load();
	if (texture != nullptr) {//si hay textura
		texture->bind(GL_MODULATE);//MODULATE para que se mezclen los colores de la luz con los de la textura (por defecto REPLACE, que solo se verian los de la text)
		gluQuadricTexture(quadric, GL_TRUE);//se le aplica la textura al objeto cuadrico
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluSphere(quadric, r, r, r);//se genera una esfera en el objeto cuadrico de radio r y densidad de vectores
		gluQuadricTexture(quadric, GL_FALSE);
		texture->unbind();
	}
	else {
		gluQuadricDrawStyle(quadric, GLU_FILL);
		gluSphere(quadric, r, r, r);
	}
}

void EsferaLuz::render(glm::dmat4 const& modelViewMat) {
	dmat4 aMat = modelViewMat * modelMat;
	// aplicar la trayectoria en aMat
	spot.load(aMat);//se pinta antes el foco
	Esfera::render(modelViewMat);
	renderHijos(aMat);//pintamos a las hijas
}

void EsferaLuz::renderHijos(glm::dmat4 modelViewMat){
	glm::dmat4 aMat;
	aMat = translate(modelViewMat, posHija);
	glLoadMatrixd(value_ptr(aMat));
	esfera1->draw();

	aMat = translate(modelViewMat, -posHija);
	glLoadMatrixd(value_ptr(aMat));
	esfera2->draw();
}

void Diabolo::render(glm::dmat4 const& modelViewMat){
	setMvM(modelViewMat);//vamos a pintar cuatro piramides formando asi un diabolo

	//primera piramide
	dmat4 auxMat = modelViewMat * modelMat;//creamos una matriz auxiliar
	auxMat = translate(auxMat, glm::dvec3(100.0, 100.0, -altura));//trasladamos en z altura para que la punta quede en el centro del eje
	auxMat = rotate(auxMat, radians(rotation), glm::dvec3(0.0, 0.0, 1.0));//la rotamos en el eje z
	glLoadMatrixd(value_ptr(auxMat));//cargamos la matriz auxiliar
	if (texture == nullptr)mesh = Mesh::generateTriPyramid(radio, altura);//creamos la piramide que recibe las transformaciones anteriores
	else mesh = Mesh::generateTriPyramidTex(radio, altura);
	draw();//la dibujamos

	//segunda
	auxMat = rotate(auxMat, radians(60.0), glm::dvec3(0.0, 0.0, 1.0));//rotamos mas en z
	glLoadMatrixd(value_ptr(auxMat));//cargamos la matriz auxiliar
	if (texture == nullptr)mesh = Mesh::generateTriPyramid(radio, altura);//piramide
	else mesh = Mesh::generateTriPyramidTex(radio, altura);
	draw();

	//tercera
	auxMat = rotate(auxMat, radians(1.0), glm::dvec3(0.0, 0.0, 1.0));//rotamos en z como la primera
	auxMat = translate(auxMat, glm::dvec3(0.0, 0.0, altura * 2));//trasladamos en z el doble de la altura para que este al otro lado
	glLoadMatrixd(value_ptr(auxMat));//cargamos la matriz auxiliar
	if (texture == nullptr)mesh = Mesh::generateTriPyramid(radio, -altura);//piramide
	else mesh = Mesh::generateTriPyramidTex(radio, -altura);
	draw();

	//cuarta
	auxMat = rotate(auxMat, radians(60.0), glm::dvec3(0.0, 0.0, 1.0));//rotamos mas en z como la segunda
	glLoadMatrixd(value_ptr(auxMat));//cargamos la matriz auxiliar
	if (texture == nullptr)mesh = Mesh::generateTriPyramid(radio, -altura);//piramide
	else mesh = Mesh::generateTriPyramidTex(radio, -altura);
	draw();
}

void ContRectangle::draw() {
	if (texture != nullptr){
		texture->bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->draw();
		texture->unbind();
	}
	else{
		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mesh->draw();
		glLineWidth(1);
	}
}

void Cubo::render(glm::dmat4 const& modelViewMat) {
	setMvM(modelViewMat);//vamos a pintar un cubo sin tapas y dos cuadrados para las tapas

	if (texture == nullptr) {
		//cubo sin tapas
		mesh = Mesh::generateContCubo(l);
		draw();

		//tapa del fondo
		mesh = Mesh::generateCuadrado(l);
		draw();

		//tapa de arriba
		dmat4 auxMat = modelViewMat * modelMat;//creamos una matriz auxiliar
		auxMat = translate(auxMat, glm::dvec3(l/2-(cos(radians(45.0))*l/2), l + (l / 2)*sin(radians(45.0)), 0.0));//trasladamos en y altura para que la tapa quede arriba del cubo
		auxMat = rotate(auxMat, radians(-45.0), glm::dvec3(0.0, 0.0, 1.0));//la rotamos en el eje z 45 grados
		glLoadMatrixd(value_ptr(auxMat));//cargamos la matriz auxiliar
		mesh = Mesh::generateCuadrado(l);
		draw();
	}
	else {
		//cubo sin tapas
		dmat4 auxMat = modelViewMat * modelMat;//creamos una matriz auxiliar
		auxMat = translate(auxMat, glm::dvec3(40.0, 0.0, 80.0));
		glLoadMatrixd(value_ptr(auxMat));//cargamos la matriz auxiliar
		mesh = Mesh::generateContCubeTex(l);
		draw();

		//tapa del fondo
		dmat4 auxMat1 = modelViewMat * modelMat;//creamos una matriz auxiliar
		auxMat1 = translate(auxMat1, glm::dvec3(40.0, 0.1, 80.0));
		auxMat1 = rotate(auxMat1, radians(-90.0), glm::dvec3(1.0, 0.0, 0.0));//la rotamos en el eje x 90 grados
		glLoadMatrixd(value_ptr(auxMat1));//cargamos la matriz auxiliar
		mesh = Mesh::generateRectangleTex(l, l, 1, 1);//creamos un rectangulo con textura
		draw();

		//tapa de arriba
		dmat4 auxMat2 = modelViewMat * modelMat;//creamos una matriz auxiliar
		auxMat2 = translate(auxMat2, glm::dvec3(40.0, 0.0, 80.0));
		auxMat2 = translate(auxMat2, glm::dvec3(l / 2 - (cos(radians(45.0))*l / 2), l + (l / 2)*sin(radians(45.0)), 0.0));//trasladamos en y altura para que la tapa quede arriba del cubo
		auxMat2 = rotate(auxMat2, radians(90.0), glm::dvec3(1.0, 0.0, 0.0));//la rotamos en el eje x 90 grados
		auxMat2 = rotate(auxMat2, radians(-45.0), glm::dvec3(0.0, 1.0, 0.0));//la rotamos en el eje y 45 grados
		glLoadMatrixd(value_ptr(auxMat2));//cargamos la matriz auxiliar
		mesh = Mesh::generateRectangleTex(l, l, 1, 1);
		draw();
	}
}

void GlassPot::render(glm::dmat4 const& modelViewMat) {
	setMvM(modelViewMat);

	//si hay textura pintamos un cubo sin tapas
	if (texture != nullptr) {
		dmat4 auxMat = modelViewMat * modelMat;//creamos una matriz auxiliar
		auxMat = translate(auxMat, glm::dvec3(250.0 - l / 2, 0.0, 250.0 - l / 2));//lo ponemos en la esquina del suelo
		glLoadMatrixd(value_ptr(auxMat));//cargamos la matriz auxiliar
		mesh = Mesh::generateContCubeTex(l);
		draw();
	}
}

void Grass::render(glm::dmat4 const& modelViewMat) {
	setMvM(modelViewMat);

	//pintamos dos rectangulos formando una "x"
	if (texture != nullptr) {
		dmat4 auxMat1 = modelViewMat * modelMat;//creamos una matriz auxiliar
		auxMat1 = translate(auxMat1, glm::dvec3(250.0 - l/2, l/2, 250.0 - l/2));//lo ponemos en la esquina del suelo
		auxMat1 = rotate(auxMat1, radians(-45.0), glm::dvec3(0.0, 1.0, 0.0));//lo rotamos en el eje y -45 grados
		glLoadMatrixd(value_ptr(auxMat1));//cargamos la matriz auxiliar
		mesh = Mesh::generateRectangleTex(l, l, 1, 1);//creamos un rectangulo con textura
		draw();

		dmat4 auxMat2 = modelViewMat * modelMat;
		auxMat2 = translate(auxMat2, glm::dvec3(250.0 - l / 2, l/2, 250.0 - l / 2));
		auxMat2 = rotate(auxMat2, radians(45.0), glm::dvec3(0.0, 1.0, 0.0));
		glLoadMatrixd(value_ptr(auxMat2));
		mesh = Mesh::generateRectangleTex(l, l, 1, 1);
		draw();
	}
}

void ContRectangle::render(glm::dmat4 const& modelViewMat) {
	if (texture != nullptr)texture->bind();
	setMvM(modelViewMat);
	dmat4 auxMat = modelViewMat * modelMat;//creamos una matriz auxiliar
	auxMat = rotate(auxMat, radians(90.0), glm::dvec3(1.0, 0.0, 0.0));//la rotamos en el eje z
	glLoadMatrixd(value_ptr(auxMat));//cargamos la matriz auxiliar
	draw();
	if (texture != nullptr)texture->unbind();
}

void Dragon::draw() {
	glPointSize(2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mesh->draw();
	glPointSize(2);
}

void Dragon::render(glm::dmat4 const& modelViewMat) {
	setMvM(modelViewMat);

	dmat4 auxMat = modelViewMat * modelMat;//creamos una matriz auxiliar
	auxMat = translate(auxMat, glm::dvec3(-40.0, -170.0, 0.0));//trasladamos en x e y
	auxMat = scale(auxMat, glm::dvec3(10, 10, 0.0));//escalamos en x e y(para que quepa en pantalla esta escalado 10 veces mas grande, no 40)
	glLoadMatrixd(value_ptr(auxMat));//cargamos la matriz auxiliar
	mesh = mesh = Mesh::generaDragon(numVert);

	draw();//lo dibujamos
}

void PoliEspiral::draw() {
	glPointSize(2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mesh->draw();
	glPointSize(2);
}

Foto::Foto(GLdouble w, GLdouble h, GLuint x, GLuint y) :Entity() {
	texture = new Texture();
	mesh = Mesh::generateRectangleTex(w, h, x, y);
}

void Foto::draw() {
		texture->bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->draw();
		texture->unbind();
}

void Foto::render(glm::dmat4 const& modelViewMat) {
	setMvM(modelViewMat);
	dmat4 auxMat = modelViewMat * modelMat;//creamos una matriz auxiliar
	auxMat = translate(auxMat, glm::dvec3(0.0, 1, 0.0));
	auxMat = rotate(auxMat, radians(90.0), glm::dvec3(1.0, 0.0, 0.0));//la rotamos en el eje z
	glLoadMatrixd(value_ptr(auxMat));//cargamos la matriz auxiliar
	draw();
}

//dibujamos el terreno
void Terreno::draw()
{
	if (texture != nullptr) {//si tiene textura la decimos que se mezcle con la luz
		texture->bind(GL_MODULATE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->draw();
		texture->unbind();
	}
	else {//si no, dibujamos los poligonos con lineas
		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mesh->draw();
		glLineWidth(1);
	}
}
//-------------------------------------------------------------------------
