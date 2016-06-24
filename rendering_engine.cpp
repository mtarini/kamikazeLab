/* rendering engine!!!
 *
 * Il nome e' un po' altisonante, ma
 * tutte le funzioni inerenti al rendering
 * la piazziamo qui
 */

#include<Windows.h>
#include<GL/GL.h>
#include<glm/mat4x4.hpp>

#include"transform.h"
#include"phys_object.h"
#include"scene.h"


void Transform::setModelMatrix() const{

	glm::mat4 rotMat = mat4_cast( ori );
	glm::mat4 traMat(1.0);
	glm::mat4 scaMat;

	scaMat[0][0] = scale;
	scaMat[1][1] = scale;
	scaMat[2][2] = scale;

	traMat[3] = vec4( pos , 1 );

	glm::mat4 m = traMat * rotMat * scaMat;

	glMultMatrixf( & (m[0][0]) );
}

void drawAStupidTriangle();

void Bullet::renderPlaceHolder() const{
	// sending the cords in world space! (optimization)
	if (alive)
		glVertex3f( t.pos.x , t.pos.y , t.pos.z );
}

void Ship::renderPlaceHolder() const{

	glPushMatrix();
	t.setModelMatrix();
	drawAStupidTriangle();

	/* TODO: se fosse un albero della scena, i figli
	 * di questo nodo vanno disegnai qui
	 * con una chiamata ricosrica alla renderPlaceHolder */

	glPopMatrix();

	glPointSize(4.0);
	glColor3f( 1,1,0 );
	glBegin(GL_POINTS);
	for (const Bullet &b: bullets){
		b.renderPlaceHolder();
	}
	glEnd();
}


void Scene::render(){

	glPushMatrix();
	// glScale: matrice_top_dello_stack *= matrice di scaling
	glScalef( 1.0f/arenaRadius, 1.0f/arenaRadius, 1.0f/arenaRadius);
	for ( Ship& s : ships){
		s.renderPlaceHolder();
	}
	glPopMatrix();

}

void drawAStupidTriangle(){
	glColor3f(1,1,1);
	glBegin(GL_TRIANGLES);
	glVertex2f(   0, 0.8f);
	glVertex2f(-0.5,-0.5);
	glVertex2f(+0.5,-0.5);
	glEnd();
}

/* metodo globale che disegna la scena */
void rendering(){
	glViewport(0,0,500,500); // TODO: use actual windows size

	glClearColor( 1.0, 0, 0.0, 1.0 );


	glClear( GL_COLOR_BUFFER_BIT );

	scene.render();
}

