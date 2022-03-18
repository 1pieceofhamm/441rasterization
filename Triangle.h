#pragma once

#include <stdlib.h>
#include <math.h>

#include <glm/glm.hpp>
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

class Triangle {
	private:
		
	public:
		glm::vec3 v[3];		// Triangle vertices
		glm::vec3 c[3];		// Vertex color
		glm::vec3 newV[3];

		// Constructor
		Triangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2, int mode);

		// Rendering the triangle using OpenGL
		void RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix);

		// Rendering the triangle using CPU
		void RenderCPU(glm::mat4& modelViewMatrix, glm::mat4& projectionMatrix, glm::mat4& viewportMatrix, float color[WINDOW_HEIGHT][WINDOW_WIDTH][3], float depth[WINDOW_HEIGHT][WINDOW_WIDTH]);

};
