#ifndef OPENGL_H
#define OPENGL_H


#pragma once
#include "stdafx.h"
// include c/c++ library
#include <iostream>
// GLEW ( help you using functions without retreiving functions )
#pragma comment (lib, "glew32s.lib")
#define GLEW_STATIC
#include <GL\glew.h>
// GLFW ( make you a windows that support opengl operation to work fine with your platform )
#include <GLFW\glfw3.h>
#include <glut.h>
#include <algorithm>
#include <vector>
#include <winsock2.h>
#include <fstream>
#include <sstream>

#include "read.h"

using namespace std;

GLfloat cameraDistance = 12, cameraAngle = 0;
bool running = true;
// angles = { X rotation at shoulder, Y rotation at shoulder, Z rotation at shoulder, Z rotation at elbow }
int arm_angles[4] = { 0, 0, 0, 0 };
enum { SHOULDER_X, SHOULDER_Y, SHOULDER_Z, ELBOW_Z };

read read_stl;

SOCKET sConnect;
SOCKADDR_IN addr;
int addrlen;

ifstream fin;

vector<triangle> facet[6];
       // define a vector object (container) called "facet" of type "triangle"
double x_min[6], x_max[6];       // geometry extents along X
double y_min[6], y_max[6];       // geometry extents along Y
double z_min[6], z_max[6];       // geometry extents along Z
float scale_0[6];

float* vertices_1;
float* vertices_2;
float* vertices_3;
float* vertices_4;
float* vertices_5;
float* vertices_6;

GLuint bufferID[6];


#endif // !OPENGL_H