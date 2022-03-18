#include "Triangle.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))


Triangle::Triangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2, int mode)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r7 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r8 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r9 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (mode == 0) {
		c[0] = glm::vec3(r1, r2, r3);
		c[1] = glm::vec3(r1, r2, r3);
		c[2] = glm::vec3(r1, r2, r3);
	}
	if (mode == 1) {
		c[0] = glm::vec3(r1, r2, r3);
		c[1] = glm::vec3(r4, r5, r6);
		c[2] = glm::vec3(r7, r8, r9);
	}
	if(mode == 2) {
		c[0] = glm::vec3(v[0].z, 0.0f, 0.0f);
		c[1] = glm::vec3(v[1].z, 0.0f, 0.0f);
		c[2] = glm::vec3(v[2].z, 0.0f, 0.0f);
	}
	//c[0] = glm::vec3(1.0f, 0.0f, 0.0f);
	//c[1] = glm::vec3(1.0f, 0.0f, 0.0f);
	//c[2] = glm::vec3(1.0f, 0.0f, 0.0f);
}

// Rendering the triangle using OpenGL
void Triangle::RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix) 
{

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(modelViewMatrix));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));

	glBegin(GL_TRIANGLES);
		glColor3f(c[0].x, c[0].y, c[0].z);
		glVertex3f(v[0].x, v[0].y, v[0].z);

		glColor3f(c[1].x, c[1].y, c[1].z);
		glVertex3f(v[1].x, v[1].y, v[1].z);

		glColor3f(c[2].x, c[2].y, c[2].z);
		glVertex3f(v[2].x, v[2].y, v[2].z);
	glEnd();
}

// Render the triangle on CPU
void Triangle::RenderCPU(glm::mat4& modelViewMatrix, glm::mat4& projectionMatrix, glm::mat4& viewportMatrix, float color[WINDOW_HEIGHT][WINDOW_WIDTH][3], float depth[WINDOW_HEIGHT][WINDOW_WIDTH])
{
	//transform triangle to screen
	glm::vec4 v0= viewportMatrix*projectionMatrix*modelViewMatrix*glm::vec4(v[0], 1);
	v0 = v0/v0.w;
	newV[0] = v0;
	//std::cout << "made it";

	glm::vec4 v1 = viewportMatrix * projectionMatrix * modelViewMatrix * glm::vec4(v[1], 1);
	v1 = v1/v1.w;
	newV[1] = v1;
	//std::cout << "made it1";

	glm::vec4 v2 = viewportMatrix * projectionMatrix * modelViewMatrix * glm::vec4(v[2], 1);
	v2 = v2/v2.w;
	newV[2] = v2;
	//std::cout << "made it2";
	
	//rasterize
	int minX = 10000; //bounding variables
	int maxX = -10000;
	int minY = 10000;
	int maxY = -10000;
	for (int k = 0; k < 3; k++) { //get min and max x and y for bounding box
		if (newV[k].x > maxX) {
			maxX = newV[k].x;
		}
		if (newV[k].x < minX) {
			minX = newV[k].x;
		}
		if (newV[k].y > maxY) {
			maxY = newV[k].y;
		}
		if (newV[k].y < minY) {
			minY = newV[k].y;
		}
	}
	if (minX <= 0) {
		minX = 0;
	}
	if (minY <= 0) {
		minY = 0;
	}
	if (maxX > WINDOW_WIDTH-1) {
		maxX = WINDOW_WIDTH-1;
	}
	if (maxY > WINDOW_HEIGHT-1) {
		maxY = WINDOW_HEIGHT-1;
	}
	float alpha = 0;
	float beta = 0;
	float gamma = 0;
	for (int j = minY-1; j < maxY+1; j++) { //looping through bounding box
		for (int i = minX-1; i < maxX+1; i++) {//v[0] = A, v[1] = B, v[2] = C
			alpha = (((-1) * ((i - newV[1].x) * (newV[2].y - newV[1].y))) + ((j - newV[1].y) * (newV[2].x - newV[1].x))) / (((-1) * (newV[0].x - newV[1].x) * (newV[2].y - newV[1].y) + (newV[0].y - newV[1].y) * (newV[2].x - newV[1].x)));
			beta = (((-1) * ((i - newV[2].x) * (newV[0].y - newV[2].y))) + ((j - newV[2].y) * (newV[0].x - newV[2].x))) / (((-1) * (newV[1].x - newV[2].x) * (newV[0].y - newV[2].y) + (newV[1].y - newV[2].y) * (newV[0].x - newV[2].x)));
			gamma = 1 - alpha - beta;
			if ((alpha >= 0 && alpha <= 1) && (beta >= 0 && beta <= 1) && (gamma >= 0 && gamma <= 1)) {//if pixel is in triangle
				//z buffer algo
				if ((alpha * newV[0].z) + (beta * newV[1].z) + (gamma * newV[2].z) < depth[(int)j][(int)i]) {
				//change color
					glm::vec3 newColor = (c[0]*alpha) + (c[1]*beta) + (c[2]*gamma);
					color[(int)j][(int)i][0] = newColor.x;
					color[(int)j][(int)i][1] = newColor.y;
					color[(int)j][(int)i][2] = newColor.z;
					depth[(int)j][(int)i] = (alpha * newV[0]).z + (beta * newV[1]).z + (gamma * newV[2]).z;
				}
				else {
					;
				}
			
			}

		}
	}
	
}