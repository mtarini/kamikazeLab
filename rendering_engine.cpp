/* rendering engine!!!
 *
 * Il nome e' un po' altisonante, ma
 * tutte le funzioni inerenti al rendering
 * la piazziamo qui
 */

#include<Windows.h>
#include<GL/glew.h>
#include<GL/GL.h>
#include<glm/mat4x4.hpp>

#include"transform.h"
#include"phys_object.h"
#include"scene.h"
#include"mesh.h"

Mesh aMesh;

void Mesh::uploadToGPU(){

	if (geomBufferId==666) glGenBuffers(1,&geomBufferId);

	glBindBuffer( GL_ARRAY_BUFFER, geomBufferId );

	glBufferData(
		GL_ARRAY_BUFFER, // a buffer containing vertices
		sizeof(Vertex)*verts.size(), // how many bytes to copy on GPU
		&(verts[0]), // location in CPU of the data to copy on GPU
		GL_STATIC_DRAW // please know that this buffer is readonly!
	);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3,GL_FLOAT,sizeof(Vertex),
					(const void*)offsetof(Vertex,pos) );

	if (connBufferId==666) glGenBuffers(1,&connBufferId);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, connBufferId );
	glEnableClientState(GL_INDEX_ARRAY);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, // a buffer containing vertex indices
		sizeof(Tri)*tris.size(),
		&(tris[0]),
		GL_STATIC_DRAW
	);
	glIndexPointer(	GL_INT, 0, 0);


}

void Mesh::fire(){
	glBindBuffer( GL_ARRAY_BUFFER, geomBufferId );
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, connBufferId );
	glEnableClientState(GL_INDEX_ARRAY);
	glDrawElements(GL_TRIANGLES, tris.size()*3, GL_UNSIGNED_INT, 0 );
}

void Mesh::render(){
	if (geomBufferId==666) uploadToGPU();
	fire();
}

void Mesh::renderDeprecated(){
	// using OpenGL 1.0
	glBegin(GL_TRIANGLES);
	for (Tri t: tris) {
		{
			Vertex &v( verts[ t.i ] );
			glVertex3f( v.pos.x, v.pos.y, v.pos.z );
		}
		{
			Vertex &v( verts[ t.j ] );
			glVertex3f( v.pos.x, v.pos.y, v.pos.z );
		}
		{
			Vertex &v( verts[ t.k ] );
			glVertex3f( v.pos.x, v.pos.y, v.pos.z );
		}
	}
	glEnd();
}

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


void Scene::cameraOnTwoObjects( const PhysObject& a, const PhysObject& b ){
	vec3 center = ( a.t.pos + b.t.pos )*0.5f;
	float radius = length( a.t.pos - b.t.pos )/2.0f + 2.0f;
	/*
	 * if it was a MODEL TRANSFORM of an object which is a camera:
	glTranslatef(center.x,center.y,center.z);
	glScalef(radius,radius,radius);
	*/
	glScalef(1/radius,1/radius,1/radius);
	glTranslatef(-center.x,-center.y,-center.z);

}

void Scene::render(){

	glPushMatrix();

	// global view:
	//glScalef( 1.0f/arenaRadius, 1.0f/arenaRadius, 1.0f/arenaRadius);

	cameraOnTwoObjects(ships[0],ships[1]);

	//aMesh.render();
	glPushMatrix();
	glRotatef(60,1,1,3);
	//aMesh.renderDeprecated();
	aMesh.render();
	glPopMatrix();
	renderFloor();


	for ( Ship& s : ships){
		s.renderPlaceHolder(); // including its bullets
	}
	glPopMatrix();
}

void Scene::renderFloor() const{
	glBegin(GL_QUADS);
	glColor3f(0.75,0,0);
	for (float x=-arenaRadius; x<arenaRadius; x+=4)
	for (float y=-arenaRadius; y<arenaRadius; y+=4) {
		glVertex2d(x  ,y);
		glVertex2d(x+2,y);
		glVertex2d(x+2,y+2);
		glVertex2d(x  ,y+2);
	}
	glEnd();
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

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	scene.render();

}

/* metodo globale che inizializza il sistema grafico */
void initRendering(){
	//aMesh.buildTorus(10,30,5.0,15.0);
	aMesh.import("C:/corsi/game_engines_2016/kamikazeLab2016/assets/dark_fighter_6.obj");
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}
