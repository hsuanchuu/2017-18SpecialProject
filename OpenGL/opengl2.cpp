// opengl.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "opengl2.h"

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
void new_angle(int angle_num, int angle, int minimum = 0, int maximum = 180) {
	arm_angles[angle_num] = angle % 360;
	arm_angles[angle_num] = max(arm_angles[angle_num], minimum);
	arm_angles[angle_num] = min(arm_angles[angle_num], maximum);
}

// Create and Display 3D model of arm
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0;i < 6;i++) {
		//Camera view
		glLoadIdentity();
		GLfloat x = cameraDistance * sin(cameraAngle);
		GLfloat z = cameraDistance * cos(cameraAngle);
		gluLookAt(x, 0.0, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		glPushMatrix();

		// transformations on the 3D object - Modeling transformations
		// change the current mode to MODELVIEW
		glMatrixMode(GL_MODELVIEW);

		if (i == 0) {
			glTranslatef(0., 0., 1.5);
			// rotate object about the X, Y and Z axes
			glRotatef((GLfloat)arm_angles[SHOULDER_X], 1.0, 0.0, 0.0);
			glRotatef((GLfloat)arm_angles[SHOULDER_Y], 0.0, 1.0, 0.0);
			glRotatef((GLfloat)arm_angles[SHOULDER_Z], 0.0, 0.0, 1.0);
		}
		else if (i == 1) {
			glTranslatef(0., 0., 1.5);
			glRotatef((GLfloat)arm_angles[SHOULDER_X], 1.0, 0.0, 0.0);
			glRotatef((GLfloat)arm_angles[SHOULDER_Y], 0.0, 1.0, 0.0);
			glRotatef((GLfloat)arm_angles[SHOULDER_Z], 0.0, 0.0, 1.0);
			glTranslatef(2.0, 0., 0.);
		}

		else if (i == 2) {
			glTranslatef(0., 0., 1.5);
			glRotatef((GLfloat)arm_angles[SHOULDER_X], 1.0, 0.0, 0.0);
			glRotatef((GLfloat)arm_angles[SHOULDER_Y], 0.0, 1.0, 0.0);
			glRotatef((GLfloat)arm_angles[SHOULDER_Z], 0.0, 0.0, 1.0);
			glTranslatef(2.0, 0., 0.);
			//glColor3f(50. / 255., 50. / 255., 255. / 255.);
			//glutSolidSphere(0.25, 10, 10);
			glRotatef((GLfloat)arm_angles[ELBOW_Z], 0.0, 0.0, 1.0);  // rotate about Z
		}
		else if (i == 3) {
			glTranslatef(0., 0., -1.5);
			/*glRotatef(-(GLfloat)arm_angles[SHOULDER_X], 1.0, 0.0, 0.0);
			glRotatef(-(GLfloat)arm_angles[SHOULDER_Y], 0.0, 1.0, 0.0);
			glRotatef((GLfloat)arm_angles[SHOULDER_Z], 0.0, 0.0, 1.0);*/
		}
		else if (i == 4) {
			glTranslatef(0., 0., -1.5);
			/*glRotatef(-(GLfloat)arm_angles[SHOULDER_X], 1.0, 0.0, 0.0);
			glRotatef(-(GLfloat)arm_angles[SHOULDER_Y], 0.0, 1.0, 0.0);
			glRotatef((GLfloat)arm_angles[SHOULDER_Z], 0.0, 0.0, 1.0);*/
			glTranslatef(2.0, 0., 0.);
		}
		else {
			glTranslatef(0., 0., -1.5);
			/*glRotatef(-(GLfloat)arm_angles[SHOULDER_X], 1.0, 0.0, 0.0);
			glRotatef(-(GLfloat)arm_angles[SHOULDER_Y], 0.0, 1.0, 0.0);
			glRotatef((GLfloat)arm_angles[SHOULDER_Z], 0.0, 0.0, 1.0);*/
			glTranslatef(2.0, 0., 0.);
			//glRotatef((GLfloat)arm_angles[ELBOW_Z], 0.0, 0.0, 1.0);  // rotate about Z
		}

		// translate the geometry along X, Y and Z
		// if the model is not at the center
		//glTranslatef(move1[0],move1[1],move1[2]);
		glPushMatrix();

		//bind different model by buffer ID
		glBindBuffer(GL_ARRAY_BUFFER, bufferID[i]);

		// Viewing transformation
		glEnable(GL_DEPTH_TEST);   // hidden surface removal
		// change the current mode to MODELVIEW
		glMatrixMode(GL_MODELVIEW);

		//since each model has the same scale
		//I use the same scale for every model
		glScalef(1.0 / (scale_0[0]), 1.0 / (scale_0[0]), 1.0 / (scale_0[0]));

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

		// draw the geometry
		glDrawArrays(GL_TRIANGLES,     // type of GL element
			0,                // starting offset
			3 * facet[i].size());    // number of vertices to be used for rendering

		//pop translation and rotation matrix
		glPopMatrix();
		//clean buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//pop camera view
		glPopMatrix();
	}
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
		fin.close();
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

void receive(int value) {
	char message[200];
	ZeroMemory(message, 200);
	int r = recvfrom(sConnect, message, sizeof(message), 0, (sockaddr*)&addr, &addrlen);
	if (r != SOCKET_ERROR) {
		string mes(message);
		cout << mes << endl;
		istringstream ss(mes);
		string token;
		int count = 0;
		while (getline(ss,token,',')) {
			stringstream num_s;
			int num;
			num_s << token;
			num_s >> num;
			if (count == 3) {
				new_angle(count, num, 0, 180);
			}
			else {
				new_angle(count, num, -90, 90);
			}
			
			//cout << arm_angles[count] << ",";
			count++;
		}
		//cout << endl;
	}
	display();
	glutTimerFunc(20, receive, 1);
}


// Program Principal
int main(int argc, char * argv[]) {
	//initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Arm");

	glewExperimental = GL_TRUE;
	if (!glewInit()) {
		cout << "initialization done" << endl;
	};
	//window color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//start reading stl file
	int error[6];
	error[0] = read_stl.input_binary(facet[0], "right_upper_arm.stl", x_min[0], x_max[0], y_min[0], y_max[0], z_min[0], z_max[0]);
	error[1] = read_stl.input_binary(facet[1],"right_elbow.stl", x_min[1], x_max[1], y_min[1], y_max[1], z_min[1], z_max[1]);
	error[2] = read_stl.input_binary(facet[2], "right_forearm.stl", x_min[2], x_max[2], y_min[2], y_max[2], z_min[2], z_max[2]);
	error[3] = read_stl.input_binary(facet[3], "left_upper_arm.stl", x_min[3], x_max[3], y_min[3], y_max[3], z_min[3], z_max[3]);
	error[4] = read_stl.input_binary(facet[4], "left_elbow.stl", x_min[4], x_max[4], y_min[4], y_max[4], z_min[4], z_max[4]);
	error[5] = read_stl.input_binary(facet[5], "left_forearm.stl", x_min[5], x_max[5], y_min[5], y_max[5], z_min[5], z_max[5]);

	for (int i = 0;i < 6;i++) {
		if (error[i] == 1) {
			cout << " unable to read stl file" << i << endl;
			getchar();
			return 1;
		}
	}

	// calculate translation parameters to move the center of 
	// the geometry approximately to the origin
	for (int i = 0;i < 6;i++) {
		// calculate scaling factors along X, Y and Z
		float scale_x = (x_max[i] - x_min[i]) / 2.0;
		float scale_y = (y_max[i] - y_min[i]) / 2.0;
		float scale_z = (z_max[i] - z_min[i]) / 2.0;

		scale_0[i] = scale_x;
		if (scale_y > scale_0[i]) scale_0[i] = scale_y;
		if (scale_z > scale_0[i]) scale_0[i] = scale_z;

		cout << "Need to scale the geometry by: " << 1.0 / scale_0[i] << endl;

		/*float x_center = (x_min[i] + x_max[i]) / 2.0;
		float y_center = (y_min[i] + y_max[i]) / 2.0;
		float z_center = (z_min[i] + z_max[i]) / 2.0;*/

		if (i == 0) {
			/*move1[0] = 0.0 - x_center;   // translation along X
			move1[1] = 0.0 - y_center;   // translation along Y
			move1[2] = 0.0 - z_center;   // translation along Z

			cout << "Need to translate the geometry1 by: " << move1[0]
				<< " " << move1[1]
				<< " " << move1[2] << endl;*/

			//create vertex array
			vertices_1 = new float[facet[i].size() * 30];
			read_stl.createVertexArray(facet[i], vertices_1);
			//end reading

			// create a Vertex Buffer Object (VBO) and bind the vertex array to it
			// makes rendering faster because data is copied to GPU memory
			glGenBuffers(1, &bufferID[i]);
			glBindBuffer(GL_ARRAY_BUFFER, bufferID[i]);
			glBufferData(GL_ARRAY_BUFFER, facet[i].size() * 30 * sizeof(GLfloat), vertices_1, GL_STATIC_DRAW);
		}
		else if (i == 1) {
			/*move2[0] = 0.0 - x_center;   // translation along X
			move2[1] = 0.0 - y_center;   // translation along Y
			move2[2] = 0.0 - z_center;   // translation along Z

			cout << "Need to translate the geometry2 by: " << move2[0]
				<< " " << move2[1]
				<< " " << move2[2] << std::endl;*/

			//create vertex array
			vertices_2 = new float[facet[i].size() * 30];
			read_stl.createVertexArray(facet[i], vertices_2);
			//end reading

			// create a Vertex Buffer Object (VBO) and bind the vertex array to it
			// makes rendering faster because data is copied to GPU memory
			glGenBuffers(1, &bufferID[i]);
			glBindBuffer(GL_ARRAY_BUFFER, bufferID[i]);
			glBufferData(GL_ARRAY_BUFFER, facet[i].size() * 30 * sizeof(GLfloat), vertices_2, GL_STATIC_DRAW);
		}
		else if (i == 2) {
			//create vertex array
			vertices_3 = new float[facet[i].size() * 30];
			read_stl.createVertexArray(facet[i], vertices_3);
			//end reading

			// create a Vertex Buffer Object (VBO) and bind the vertex array to it
			// makes rendering faster because data is copied to GPU memory
			glGenBuffers(1, &bufferID[i]);
			glBindBuffer(GL_ARRAY_BUFFER, bufferID[i]);
			glBufferData(GL_ARRAY_BUFFER, facet[i].size() * 30 * sizeof(GLfloat), vertices_3, GL_STATIC_DRAW);
		}
		else if(i==3) {
			//create vertex array
			vertices_4 = new float[facet[i].size() * 30];
			read_stl.createVertexArray(facet[i], vertices_4);
			//end reading

			// create a Vertex Buffer Object (VBO) and bind the vertex array to it
			// makes rendering faster because data is copied to GPU memory
			glGenBuffers(1, &bufferID[i]);
			glBindBuffer(GL_ARRAY_BUFFER, bufferID[i]);
			glBufferData(GL_ARRAY_BUFFER, facet[i].size() * 30 * sizeof(GLfloat), vertices_4, GL_STATIC_DRAW);

		}
		else if (i == 4) {
			//create vertex array
			vertices_5 = new float[facet[i].size() * 30];
			read_stl.createVertexArray(facet[i], vertices_5);
			//end reading

			// create a Vertex Buffer Object (VBO) and bind the vertex array to it
			// makes rendering faster because data is copied to GPU memory
			glGenBuffers(1, &bufferID[i]);
			glBindBuffer(GL_ARRAY_BUFFER, bufferID[i]);
			glBufferData(GL_ARRAY_BUFFER, facet[i].size() * 30 * sizeof(GLfloat), vertices_5, GL_STATIC_DRAW);

		}
		else {
			//create vertex array
			vertices_6 = new float[facet[i].size() * 30];
			read_stl.createVertexArray(facet[i], vertices_6);
			//end reading

			// create a Vertex Buffer Object (VBO) and bind the vertex array to it
			// makes rendering faster because data is copied to GPU memory
			glGenBuffers(1, &bufferID[i]);
			glBindBuffer(GL_ARRAY_BUFFER, bufferID[i]);
			glBufferData(GL_ARRAY_BUFFER, facet[i].size() * 30 * sizeof(GLfloat), vertices_6, GL_STATIC_DRAW);

		}

	}
	
	//construct server
	WSAData wsaData;
	WORD version = MAKEWORD(2, 2);
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "WinSock initialization fail" << endl;
		getchar();
		return 1;
	}
	//construct socket
	sConnect = socket(AF_INET, SOCK_DGRAM, 0);

	//set addr information
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addrlen = sizeof(addr);
	if (bind(sConnect, (struct sockaddr *)&addr, sizeof(addr)) != SOCKET_ERROR) {
		cout << "connect" << endl;
	}

	// fill mode or wireframe mode
	glPolygonMode(GL_FRONT,  // options: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
		GL_FILL);           // options: GL_POINT, GL_LINE, GL_FILL (default)

							// shading model
	glShadeModel(GL_SMOOTH);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);
	
	//call receive function every 20ms
	//receive angle data from client
	//if no client, delete the below line
	//glutTimerFunc(20, receive, 1);

	glutMainLoop();
}