/* rendering engine
 *
 * all methods of all classes, and all global functions, which deal with graphics:
 * rendering, uploading stuff to GPU, etc
 *
 */

#include<Windows.h>

// We use OpenGL (with glew) but the rest of the code is fairly independent from this.
// It should be trivial to switch to, e.g., directX

#include<GL/glew.h>
#include<GL/GL.h>
#include<glm/mat4x4.hpp>

#include"transform.h"
#include"phys_object.h"
#include"custom_classes.h"
#include"mesh.h"
#include"texture.h"

GpuMesh aMesh;

GpuMesh CpuMesh::uploadToGPU(){

	GpuMesh res;

	glGenBuffers(1, & res.geomBufferId );

	glBindBuffer( GL_ARRAY_BUFFER, res.geomBufferId );

	glBufferData(
		GL_ARRAY_BUFFER, // a buffer containing vertices
		sizeof(Vertex)*verts.size(), // how many bytes to copy on GPU
		&(verts[0]), // location in CPU of the data to copy on GPU
		GL_STATIC_DRAW // please know that this buffer is readonly!
	);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,
					sizeof(Vertex),
					(const void*)offsetof(Vertex,pos) );

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,
					  sizeof(Vertex),
					  (const void*)offsetof(Vertex,uv)  );


	glGenBuffers(1,&res.connBufferId);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, res.connBufferId );
	glEnableClientState(GL_INDEX_ARRAY);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, // a buffer containing vertex indices
		sizeof(Tri)*tris.size(),
		&(tris[0]),
		GL_STATIC_DRAW
	);
	//glIndexPointer(	GL_INT, 0, 0);

	res.nElements = tris.size() * 3;

	return res;
}

void GpuTexture::bind() const{
	glBindTexture( GL_TEXTURE_2D, textureId );

	// dear FIXED FUNCTIONALITY shaders, please access my texture (per fragment)
	glEnable(GL_TEXTURE_2D);
}

GpuTexture CpuTexture::uploadToGPU() const{

	GpuTexture res;
	glGenTextures(1, &res.textureId );

	glBindTexture( GL_TEXTURE_2D, res.textureId );

	glTexImage2D(
		GL_TEXTURE_2D,
		0, // higest res mipmap level
		GL_RGB,
		sizeX, sizeY,
		0,
		GL_RGBA, // becasue our class Texel is made like this
		GL_UNSIGNED_BYTE, // idem
		&(data[0])
	);

	// let's determine how this texture will be accessed (by the fragment shader)!
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	return res;
}

void GpuMesh::render() const{

	// "caro opengl, ti ricodi di quel buffer (che mi hai detto di chiamare "buffer N. geomBufferId?"
	glBindBuffer( GL_ARRAY_BUFFER, geomBufferId );
	// "ebbene, li ci troverai le (posizioni dei) VERTICI (nel modo che ti ho detto prima)"
	glEnableClientState(GL_VERTEX_ARRAY);
	// "e li ci troverai anche le (posiizoni delle) COORD TEXGURE (sempre nel modo di cui ti dissi)"
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, connBufferId );
	glEnableClientState(GL_INDEX_ARRAY);

	glDrawElements(GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0 );
}

void CpuMesh::renderDeprecated(){
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

void PhysObject::render() const{
	glColor3f(1,1,1);

	// set the local transform
	glPushMatrix();
	t.setModelMatrix();
	meshComponent.t.setModelMatrix();
	meshComponent.texture.bind();
	meshComponent.mesh.render();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void drawAStupidTriangle();

void Bullet::renderPlaceHolder() const{
	// sending the cords in world space! (optimization)
	if (alive)
		glVertex3f( t.pos.x , t.pos.y , t.pos.z );
}

void Ship::render() const{
	PhysObject::render();

	glPointSize(4.0);
	glColor3f( 1,1,0 );
	glBegin(GL_POINTS);
	for (const Bullet &b: bullets){
		b.renderPlaceHolder();
	}
	glEnd();
}

void PhysObject::setCameraInside(){

	t.inverse().setModelMatrix();
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

// this functions is an exact copy of gluPerspective (of GLU library)
// added here to avoid a dependency.
static void myGluPerspective(GLdouble fovx, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble xMin, xMax, yMin, yMax;

	xMax = zNear * tan(fovx * 3.1415926535 / 360.0);
	xMin = -xMax;

	yMin = xMin / aspect;
	yMax = xMax / aspect;

	GLdouble m[16] = {
		(2.0 * zNear) / (xMax - xMin), 0, 0, 0,

		0, (2.0 * zNear) / (yMax - yMin), 0, 0,

		(xMax + xMin) / (xMax - xMin),
		(yMax + yMin) / (yMax - yMin),
		-(zFar + zNear) / (zFar - zNear),
		-1,

		0, 0, -(2.0 * zFar * zNear) / (zFar - zNear), 0
	};

	glMultMatrixd(m);
}


void Scene::render(){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	myGluPerspective(60,1.0,0.2,100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-2); // take a step behind

	if (0) {
		// 3rd person camera
		glTranslatef(0,-2,-4);
		glRotatef(-75,1,0,0);
		ships[0].setCameraInside();
	} else {
		// bird-eye camera
		cameraOnTwoObjects(ships[0],ships[1]);
	}
	glPushMatrix();

	//aMesh.render();
	glPushMatrix();
	glRotatef(60,1,1,3);
	//aMesh.renderDeprecated();
	aMesh.render();
	glPopMatrix();
	renderFloor();


	for ( Ship& s : ships){
		s.render(); // including its bullets
	}
	glPopMatrix();
}

void Scene::renderFloor() const{
	glPushMatrix();
	glTranslatef(0,0,-2);
	glBegin(GL_QUADS);
	glColor3f(0.3f,0.2f,0);
	for (float x=-arenaRadius; x<arenaRadius; x+=4)
	for (float y=-arenaRadius; y<arenaRadius; y+=4) {
		glVertex2d(x  ,y);
		glVertex2d(x+2,y);
		glVertex2d(x+2,y+2);
		glVertex2d(x  ,y+2);
	}
	glEnd();
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

	glClearColor( 0.2f, 0.15f, 0.0f, 1.0f );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	scene.render();

}

/* metodo globale che inizializza il sistema grafico */
void initRendering(){
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}
