//#include <Windows.h>
//#include <gl/GL.h>
//#include <gl/GLU.h>
//#include <GL/glut.h>

#include <GL/freeglut.h>

#include "Camera.h"
#include "Scene.h"

#include <iostream>
using namespace std;

//---------- Global variables -------------------------------------------------------------

// Viewport position and size
Viewport viewPort(800, 600);   

// Camera position, view volume and projection
Camera camera(&viewPort);    

// Scene entities
Scene scene(&camera);   

GLuint last_update_tick;

glm::dvec2 mCoord;
//----------- Callbacks ----------------------------------------------------

void display();
void update();
void resize(int newWidth, int newHeight);
void key(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

//-------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  cout << "Starting console..." << '\n';

  // Initialization
  glutInit(&argc, argv);

  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);   
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); 
  glutInitWindowSize(800, 600);   // window size
  //glutInitWindowPosition (140, 140);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);   // | GLUT_STENCIL  
  
  int win = glutCreateWindow( "Freeglut-project" );  // window's identifier
  
  // Callback registration
  glutReshapeFunc(resize);
  glutKeyboardFunc(key);
  glutSpecialFunc(specialKey);
  glutDisplayFunc(display);
  glutIdleFunc(update);
  glutMouseFunc(mouse);//rotacion de la camara
  glutMotionFunc(motion);//rotacion de la camara
 
  cout << glGetString(GL_VERSION) << '\n';
  cout << glGetString(GL_VENDOR) << '\n';
 
  scene.init();    // after creating the context
  last_update_tick = 0;
  glutMainLoop(); 
    
  //cin.sync();   cin.get();
  glutDestroyWindow(win);  // Destroy the context 

  return 0;
}
//-------------------------------------------------------------------------

void update(){
	GLuint deltaTime = glutGet(GLUT_ELAPSED_TIME) - last_update_tick;

	if (deltaTime > 16){
		scene.update(deltaTime);
		last_update_tick = glutGet(GLUT_ELAPSED_TIME);

		display();
	}
}

//actualiza las coordenadas del raton invirtiendo el eje y; callback llamado cada vez que clicamos
void mouse(int button, int state, int x, int y){
	mCoord = glm::dvec2(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
}

//calcula el desplazamiento del raton al moverlo y manda a la camara moverse en funcion de el
//callback llamado cada vez que movemos el raton. Solo manda mover a la camara cuando estamos clicando, 
//momento en el cual se actualiza mCoord (se llama a mouse) y se produce un desplazamiento
void motion(int x, int y) {
	glm::dvec2 mOffset = mCoord; // var. global (mCoord vale las coordenadas del raton)
	mCoord = glm::dvec2(x, glutGet(GLUT_WINDOW_HEIGHT) - y);//actualizamos mCoord invirtiendo el eje y
	mOffset = (mCoord - mOffset) * 0.15; // sensitivity. mOffset vale la diferencia (desplazamiento -> nuevas coord del raton - coord anteriores)
	camera.rotatePY(mOffset.y, mOffset.x);//mandamos que se mueva con el desplazamiento
	glutPostRedisplay();//renderiza la escena
}

void display()   // double buffer
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
  
  scene.render();   
    
  glutSwapBuffers();  
}
//-------------------------------------------------------------------------

void resize(int newWidth, int newHeight)
{
  // Resize Viewport 
  viewPort.setSize(newWidth, newHeight);  
  // Resize Scene Visible Area 
  camera.setSize(viewPort.getW(), viewPort.getH());    // scale unchanged
}
//-------------------------------------------------------------------------

void key(unsigned char key, int x, int y)
{
  bool need_redisplay = true;

  switch (key) {
  case 27:  // Escape key 
    glutLeaveMainLoop();  // Freeglut's sentence for stopping glut's main loop 
    break;
  case '+': 
    camera.scale(+0.01);   // zoom in 
    break;
  case '-':
    camera.scale(-0.01);   // zoom out
    break;
  case 'l':
	  camera.set3D(); 
	  break;
  case 'o':
	  camera.setAZ();
	  break;
  case 'f'://si pulsamos "f" sacamos una foto
	  scene.textureFoto.loadColorBuffer(camera.getVP()->getW(), camera.getVP()->getH());//cargamos en una textura el Color Buffer actual (lo que hay en pantalla)
	  scene.textureFoto.save("..//Bmps/foto.bmp");//guardamos en un buffer la textura anterior y la informacion de ese buffer en "..."
	  break;
  case 'w'://si pulsamos "w" acercamos la camara
	  camera.moveFB(10);
	  break;
  case 's'://"s" la alejamos
	  camera.moveFB(-10);
	  break;
  case 'd'://derecha
	  camera.moveLR(10);
	  break;
  case 'a'://izquierda
	  camera.moveLR(-10);
	  break;
  case 'q'://arriba
	  camera.moveUD(10);
	  break;
  case 'e'://abajo
	  camera.moveUD(-10);
	  break;
  case 'p'://alternamos entre ortogonal y perspectiva
	  camera.setPrj();
	  break;
  case 'b':
	  scene.spotOnOff();
	  break;
  case 'n':
	  scene.directionalOnOff();
	  break;
  case 'm':
	  scene.esferaLuzSpotOnOff();
	  break;
  default:
    need_redisplay = false;
    break;
  }//switch

  if (need_redisplay)
	  glutPostRedisplay();
}
//-------------------------------------------------------------------------

void specialKey(int key, int x, int y)
{
  bool need_redisplay = true;

  switch (key) {
  case GLUT_KEY_RIGHT:
    camera.pitch(-1);   // rotate 1 on the X axis
	scene.RotateDiabolo(false);
    break;
  case GLUT_KEY_LEFT:
    camera.yaw(1);     // rotate 1 on the Y axis 
	scene.RotateDiabolo(false);
    break;
  case GLUT_KEY_UP:
    camera.roll(1);    // rotate 1 on the Z axis
	scene.RotateDiabolo(false);
    break;
  case GLUT_KEY_DOWN:
    camera.roll(-1);   // rotate -1 on the Z axis
	scene.RotateDiabolo(false);
    break;
  default:
    need_redisplay = false;
	scene.RotateDiabolo(false);
    break;
  }//switch

  if (need_redisplay)
	  glutPostRedisplay();
}
//-------------------------------------------------------------------------

