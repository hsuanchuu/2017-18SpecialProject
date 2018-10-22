// opengl.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "opengl.h"

using namespace std;

/*GLfloat cameraDistance = 12, cameraAngle = 0;
bool running = true;
// angles = { X rotation at shoulder, Y rotation at shoulder, Z rotation at shoulder, Z rotation at elbow }
int arm_angles[4] = { 0, 0, 0, 0 };
enum { SHOULDER_X, SHOULDER_Y, SHOULDER_Z, ELBOW_Z };*/

#define M_PI 3.14159265359


void change_angle(int angle, int delta, int minimum = 0, int maximum = 180) {
	arm_angles[angle] = (arm_angles[angle] + delta) % 360;
	arm_angles[angle] = max(arm_angles[angle], minimum);
	arm_angles[angle] = min(arm_angles[angle], maximum);
}

void draw_cylinder(GLfloat radius,
	GLfloat height,
	GLubyte R,
	GLubyte G,
	GLubyte B)
{
	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat angle = 0.0;
	GLfloat angle_stepsize = 0.5;

	/** Draw the tube */
	glColor3ub(R - 40, G - 40, B - 40);
	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	while (angle < 2 * 3.14) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		glVertex3f(x, y, 0.0);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glVertex3f(radius, 0.0, 0.0);
	glEnd();

	/** Draw the circle on top of cylinder */
	glColor3ub(R, G, B);
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * 3.14) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, 0);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, 0);

	angle = 0.0;
	while (angle < 2 * 3.14) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glEnd();
}


// Create and Display 3D model of arm
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	GLfloat x = cameraDistance * sin(cameraAngle);
	GLfloat z = cameraDistance * cos(cameraAngle);
	gluLookAt(x, 0.0, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


	glPushMatrix();
	glRotatef((GLfloat)arm_angles[SHOULDER_X], 1., 0., 0.);
	glRotatef((GLfloat)arm_angles[SHOULDER_Y], 0., 1., 0.);
	glRotatef((GLfloat)arm_angles[SHOULDER_Z], 0., 0., 1.);

	glTranslatef(1., 0., 0.);
	glPushMatrix();
	/*glScalef(2.0, 0.4, 1.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCube(1.);       // shoulder*/
	glRotatef(90.0, 0., 1., 0.);
	draw_cylinder(0.2, 1.0, 255, 160, 100);
	glPopMatrix();

	/*glTranslatef(1., 0., 0.);
	glRotatef((GLfloat)arm_angles[ELBOW_Z], 0., 0., 1.);
	glTranslatef(1., 0., 0.);
	glPushMatrix();
	glScalef(2.0, 0.4, 1.0);
	glColor3f(249. / 255., 238. / 255., 153. / 255.);
	glutWireCube(1.);       // elbow
	glPopMatrix();

	glTranslatef(1., 0., 0.);
	glPushMatrix();
	glScalef(1.0, 0.4, 1.0);
	glColor3f(207. / 255., 106. / 255., 76. / 255.);
	glutWireCube(1.);       // wrist
	glPopMatrix();
	glPopMatrix();*/

	glEnable(GL_DEPTH_TEST);   // hidden surface removal

	//------------------------------------------------------------
	// transformations on the 3D object - Modeling transformations
	//------------------------------------------------------------

	// change the current mode to MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	// current transformation matrix = identity matrix
	//glLoadIdentity();
	
	glTranslatef(1., -0.015, 0.25);
	// rotate object about the X, Y and Z axes
	//glRotatef(90, 0.0, 1.0, 0.0);  // rotate about Y
	glRotatef(90, 1.0, 0.0, 0.0);  // rotate about x

	glRotatef((GLfloat)arm_angles[ELBOW_Z]-70, 0.0, 1.0, 0.0);  // rotate about Z

	glRotatef(170, 0.0, 0.0, 1.0);  // rotate about Z
									  // translate the geometry along X, Y and Z
									  // such that it is centered at the origin
	glTranslatef(1.3, 0., -5.5);

	glPushMatrix();
	// ----------------------
	// Viewing transformation
	// ----------------------

	// change the current mode to MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	// current transformation matrix = identity matrix
	//glLoadIdentity();
	glScalef(1.0 / (scale_0), 1.0 / (scale_0), 1.0 / (scale_0));
	

	// enable use of vertex coordinate information from the array
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3,                 // number of coordinates per vertex (X,Y,Z)
		GL_FLOAT,          // type of numbers
		sizeof(float) * 10,  // stride - gap between each set of (X,Y,Z)
		(GLvoid*)((char*)NULL));  // offset - location of initial (X,Y,Z)

								  // enable use of vertex normal information from the array
	glEnableClientState(GL_NORMAL_ARRAY);

	glNormalPointer(GL_FLOAT,         // type of values
		sizeof(float) * 10, // stride - gap between each set of (N_x,N_y,N_z) 
		(GLvoid*)(((char*)NULL) + 12));  // offset - location of initial (N_x,N_y,N_z)

										 // enable use of vertex color information from the array
	glEnableClientState(GL_COLOR_ARRAY);

	glColorPointer(4,                 // number of color values per vertex (R,G,B,A)
		GL_FLOAT,          // type of values
		sizeof(float) * 10,  // stride - gap between each set of (R,G,B,A)
		(GLvoid*)(((char*)NULL) + 24));  // offset - location of initial (R,G,B,A)

										 //------------------
										 // draw the geometry
										 //------------------

										 // draw command
	glDrawArrays(GL_TRIANGLES,     // type of GL element
		0,                // starting offset
		3 * facet.size());    // number of vertices to be used for rendering

	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}



void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 300.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// adjust camera distance
void specialKeys(int key, int x, int y) {
	GLfloat distanceDelta = 1.0, angleDelta = 5 * M_PI / 180.0;
	if (key == GLUT_KEY_UP) {
		cameraDistance -= distanceDelta;
		cameraDistance = max((GLfloat)2.0, cameraDistance);
	}
	if (key == GLUT_KEY_DOWN) {
		cameraDistance += distanceDelta;
	}
	if (key == GLUT_KEY_LEFT) {
		cameraAngle -= angleDelta;
	}
	if (key == GLUT_KEY_RIGHT) {
		cameraAngle += angleDelta;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	int delta = 5;

	switch (key) {
	case 27: //ESC
		exit(0);
	case 'q':
		change_angle(SHOULDER_X, delta, -90, 90);
		cout << 'q' << endl;
		break;
	case 'a':
		change_angle(SHOULDER_X, -delta, -90, 90);
		break;
	case 'w':
		change_angle(SHOULDER_Y, delta, -90, 90);
		break;
	case 's':
		change_angle(SHOULDER_Y, -delta, -90, 90);
		break;
	case 'e':
		change_angle(SHOULDER_Z, delta, -90, 90);
		break;
	case 'd':
		change_angle(SHOULDER_Z, -delta, -90, 90);
		break;
	case 'r':
		change_angle(ELBOW_Z, delta, 0, 180);
		break;
	case 'f':
		change_angle(ELBOW_Z, -delta, 0, 180);
		break;
	}
	glutPostRedisplay();
}


// Program Principal
int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Arm");


	glewExperimental = GL_TRUE;
	if (!glewInit()) {
		cout << "initialization done" << endl;
	};

	// Initialization
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//start read stl file
	int error = read_stl.input_binary(facet, x_min, x_max, y_min, y_max, z_min, z_max);
	if (error == 1) {
		cout << "unable to read stl file" << endl;
		getchar();
		return 1;
	}

	//-------------------------------------------------------
	// calculate translation parameters to move the center of 
	// the geometry approximately to the origin
	//-------------------------------------------------------
	//cout << "main x_min:" << x_min << " x_max:" << x_max << endl;
	float x_center = (x_min + x_max) / 2.0;
	float y_center = (y_min + y_max) / 2.0;
	float z_center = (z_min + z_max) / 2.0;

	move1[0] = 0.0 - x_center;   // translation along X
	move1[1] = 0.0 - y_center;   // translation along Y
	move1[2] = 0.0 - z_center;   // translation along Z

	std::cout << "Need to translate the geometry by: " << move1[0]
		<< " " << move1[1]
		<< " " << move1[2] << std::endl;

	// calculate scaling factors along X, Y and Z
	float scale_x = (x_max - x_min) / 2.0;
	float scale_y = (y_max - y_min) / 2.0;
	float scale_z = (z_max - z_min) / 2.0;

	scale_0 = scale_x;
	if (scale_y > scale_0) scale_0 = scale_y;
	if (scale_z > scale_0) scale_0 = scale_z;

	cout << "Need to scale the geometry by: " << 1.0 / scale_0 << endl;

	//create vertex array
	vertices = new float[facet.size() * 30];
	read_stl.createVertexArray(facet, vertices);
	//end reading

	// create a Vertex Buffer Object (VBO) and bind the vertex array to it
	// makes rendering faster because data is copied to GPU memory
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, facet.size() * 30 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	// fill mode or wireframe mode
	glPolygonMode(GL_FRONT,  // options: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
		GL_FILL);           // options: GL_POINT, GL_LINE, GL_FILL (default)

							// shading model
	glShadeModel(GL_SMOOTH);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);
	

	glutMainLoop();
}