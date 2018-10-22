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

#include "read.h"

using namespace std;

GLfloat cameraDistance = 12, cameraAngle = 0;
bool running = true;
// angles = { X rotation at shoulder, Y rotation at shoulder, Z rotation at shoulder, Z rotation at elbow }
int arm_angles[4] = { 0, 0, 0, 0 };
enum { SHOULDER_X, SHOULDER_Y, SHOULDER_Z, ELBOW_Z };

read read_stl;

vector<triangle> facet;        // define a vector object (container) called "facet" of type "triangle"
double x_min, x_max, delta_x;       // geometry extents along X
double y_min, y_max, delta_y;       // geometry extents along Y
double z_min, z_max, delta_z;       // geometry extents along Z
float scale_0;
float move1[3];

float* vertices;

#endif // !OPENGL_H