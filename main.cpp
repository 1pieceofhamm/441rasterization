#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Triangle.h"


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

GLFWwindow *window;

float color[WINDOW_HEIGHT][WINDOW_WIDTH][3];
float depth[WINDOW_HEIGHT][WINDOW_WIDTH];
void setMinMaxZ();
void ClearTriangleVector();
void CreateTriangleVector(std::vector<glm::vec3>& vertices);
std::vector<glm::vec3> vertices;
std::vector<Triangle> triangleVector;
bool isOpenGL = true;
float eyeDistance = 5.0f;
int m = 0;
int minZ = 10000;
int maxZ = 10000;
std::string filepath = "../obj/";
std::string objToShow = "";



void ClearFrameBuffer()
{
	memset(&color[0][0][0], 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
}



void Display()
{	
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.0f);
	glm::mat4 modelViewMatrix = glm::lookAt(eyeDistance * glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 viewportMatrix(float(WINDOW_WIDTH / 2), 0.0f, 0.0f, 0.0f, 0.0f, float(WINDOW_HEIGHT / 2), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, float(WINDOW_WIDTH / 2), float(WINDOW_HEIGHT / 2), 0.0f, 1.0f);
	//viewportMatrix[0] = glm::vec4(float(WINDOW_WIDTH / 2), 0.0f, 0.0f, float(WINDOW_WIDTH / 2));
	//viewportMatrix[1] = glm::vec4(0.0f, float(WINDOW_HEIGHT / 2), 0.0f, float(WINDOW_HEIGHT / 2));
	//viewportMatrix[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	//viewportMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < WINDOW_HEIGHT; i++) { //initialize depth to be large
		for (int j = 0; j < WINDOW_WIDTH; j++) {
			depth[i][j] = 10000;
		}
	}
	for (int i = 0; i < WINDOW_HEIGHT; i++) { //initialize depth to be large
		for (int j = 0; j < WINDOW_WIDTH; j++) {
			color[i][j][0] = 0;
			color[i][j][1] = 0;
			color[i][j][2] = 0;
		}
	}

	if (isOpenGL)
	{
		for (int i = 0; i < triangleVector.size(); i++)
			triangleVector[i].RenderOpenGL(modelViewMatrix, projectionMatrix);
	}
	else
	{
		for (int i = 0; i < triangleVector.size(); i++)
			triangleVector[i].RenderCPU(modelViewMatrix,projectionMatrix,viewportMatrix,color,depth);

		glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, &color[0][0][0]);
	}
}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key)
	{
	case '0':
		m = 0;
		ClearTriangleVector();
		CreateTriangleVector(vertices);
		break;
	case '1':
		m = 1;
		ClearTriangleVector();
		CreateTriangleVector(vertices);
		break;
	case '2':
		m = 2;
		ClearTriangleVector();
		CreateTriangleVector(vertices);
		setMinMaxZ();
		break;
	case 'w':
		eyeDistance *= (1 - 0.05);
		break;
	case 's':
		eyeDistance *= (1 + 0.05);
		break;
	case ' ':
		isOpenGL = !isOpenGL;
		break;
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	default:
		break;
	}
}

void setMinMaxZ() {
	for (int i = 0; i < triangleVector.size(); i++) { //go thru and find min and max z value
		if (triangleVector.at(i).v[0].z < minZ) {
			minZ = triangleVector.at(i).v[0].z;
		}
		if (triangleVector.at(i).v[0].z > maxZ) {
			maxZ = triangleVector.at(i).v[0].z;
		}
		if (triangleVector.at(i).v[1].z < minZ) {
			minZ = triangleVector.at(i).v[1].z;
		}
		if (triangleVector.at(i).v[1].z > maxZ) {
			maxZ = triangleVector.at(i).v[1].z;
		}
		if (triangleVector.at(i).v[2].z < minZ) {
			minZ = triangleVector.at(i).v[2].z;
		}
		if (triangleVector.at(i).v[2].z > maxZ) {
			maxZ = triangleVector.at(i).v[2].z;
		}
	}
	int scaledDown = 0 - minZ; //gets the scalar to add to each vertex z
	int difference = maxZ - minZ; //gets scalar to divide by 
	for (int i = 0; i < triangleVector.size(); i++) { //go thru and adjust each z value
		triangleVector.at(i).v[0].z = (triangleVector.at(i).v[0].z+scaledDown)/difference;
		triangleVector.at(i).v[1].z = (triangleVector.at(i).v[1].z + scaledDown) / difference;
		triangleVector.at(i).v[2].z = (triangleVector.at(i).v[2].z + scaledDown) / difference;
	}

	//should have min and max z by now
}
void ClearTriangleVector() {
	triangleVector.clear();
}
void CreateTriangleVector(std::vector<glm::vec3> &vertices)
{
	for (int i = 0; i < vertices.size() / 3; i++)
	{
		Triangle myTriangle(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2], m);
		triangleVector.push_back(myTriangle);
	}
}

void LoadModel(char* name, std::vector<glm::vec3> &vertices)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertices.push_back(glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
												 attrib.vertices[3 * idx.vertex_index + 1],
												 attrib.vertices[3 * idx.vertex_index + 2]));
				}
				index_offset += fv;
			}
		}
	}
}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment3 - Caleb Hammack", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	ClearFrameBuffer();
	
	
	//LoadModel("../obj/bunny.obj", vertices);
	//(char)filepath;
	char* c = const_cast<char*>(filepath.c_str());
	LoadModel(c, vertices);
	CreateTriangleVector(vertices);
	
}

int main()
{	
	std::cout << "Enter the object: ";
	std::cin >> objToShow;
	filepath += objToShow;
	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
		//CreateTriangleVector(vertices);
	}

	glfwTerminate();
	return 0;
}